#include "SubKKT.inl"


namespace sofa {
namespace component {
namespace linearsolver {


typedef SubKKT::rmat rmat;

// build a projection basis based on filtering matrix P
// P must be diagonal with 0, 1 on the diagonal
void SubKKT::projection_basis(rmat& res, const rmat& P, bool identity_hint) {

    res.resize(P.rows(), P.nonZeros());

    if( identity_hint ) {
        res.setIdentity();
        return;
    }

    res.setZero();
    res.reserve(P.nonZeros());
    
    unsigned off = 0;
    for(unsigned i = 0, n = P.rows(); i < n; ++i) {

        res.startVec(i);

        if( rmat::InnerIterator(P, i) ) res.insertBack(i, off++) = 1;

    }
    
    res.finalize();
}



// builds the projected primal system P^T H P, where P is the primal
// projection basis
static void filter_primal(rmat& res,
                          const rmat& H,
                          const rmat& P,
                          bool identity_hint = false) {

    if( identity_hint ) {
        res = H;
        return;
    }
    
    res.resize(P.cols(), P.cols());
    res.setZero();
    res.reserve(H.nonZeros());

    for(unsigned i = 0, n = P.rows(); i < n; ++i) {
        for(rmat::InnerIterator it(P, i); it; ++it) {
            // we have a non-zero row in P, hence in res at row
            // it.col()
            res.startVec(it.col());

            for(rmat::InnerIterator itH(H, i); itH; ++itH) {
                
                for(rmat::InnerIterator it2(P, itH.col()); it2; ++it2) {
                    // we have a non-zero row in P, non-zero col in
                    // res at col it2.col()
                    res.insertBack(it.col(), it2.col()) = itH.value();
                }
                
            }
            
            
        }
        
    }
    res.finalize();
}


static inline bool has_row(unsigned row, const rmat& P, unsigned* col, bool identity_hint=false) {
    if(identity_hint) {
        *col = row;
        return true;
    } else {
        rmat::InnerIterator it(P, row);

        if( it ) {
            *col = it.col();
            return true;
        } else {
            return false;
        }
    }
}

// build a projected KKT system based on primal projection matrix P
// and dual projection matrix Q (to only include bilateral constraints)
void SubKKT::filter_kkt(rmat& res,
                       const rmat& H,
                       const rmat& P,
                       const rmat& Q,
                       const rmat& J,
                       const rmat& C,
                       SReal eps,
                       bool P_is_identity,
                       bool Q_is_identity,
                       bool only_lower) {


    const size_t size = P.cols() + Q.cols(); // free independent dofs + bilateral constraint dofs
    res.resize(size,size);
    res.setZero();
    res.reserve(H.nonZeros() + 2 * J.nonZeros() + C.nonZeros());

    rmat JT;

    if( !only_lower ) {
        JT = J.transpose();
    }

    const unsigned P_cols = P.cols();

    for(unsigned i = 0, n = P.rows(); i < n; ++i) {

        unsigned sub_row = 0;
        if( !has_row(i, P, &sub_row, P_is_identity) ) continue;

        res.startVec(sub_row);

        // H
        for(rmat::InnerIterator itH(H, i); itH; ++itH) {

            if( only_lower && itH.col() > itH.row() ) break;

            unsigned sub_col = 0;
            if(! has_row(itH.col(), P, &sub_col, P_is_identity) ) continue;

            res.insertBack(sub_row, sub_col) = itH.value();
        }

        if( only_lower ) continue;

        // JT
        for(rmat::InnerIterator itJT(JT, i); itJT; ++itJT) {

            unsigned sub_col = 0;
            if( !has_row(itJT.col(), Q, &sub_col, Q_is_identity) ) continue;

            res.insertBack(sub_row, P_cols + sub_col) = -itJT.value();
        }

    }

    for(unsigned i = 0, n = J.rows(); i < n; ++i) {

        unsigned sub_row = 0;
        if( !has_row(i, Q, &sub_row, Q_is_identity) ) continue;

        res.startVec(P_cols + sub_row);

        // J
        for(rmat::InnerIterator itJ(J, i); itJ; ++itJ) {

            unsigned sub_col = 0;
            if(!has_row(itJ.col(), P, &sub_col, P_is_identity)) continue;

            res.insertBack(P_cols + sub_row, sub_col) = -itJ.value();
        }

        // C
        SReal* diag = 0;
        for(rmat::InnerIterator itC(C, i); itC; ++itC) {

            if( only_lower && itC.col() > itC.row() ) break;

            unsigned sub_col = 0;
            if(!has_row(itC.col(), Q, &sub_col, Q_is_identity)) continue;

            SReal& ref = res.insertBack(P_cols + sub_row, P_cols + sub_col);
            ref = -itC.value();

            // store diagonal ref
            if(itC.col() == itC.row()) diag = &ref;
        }

        if( eps ) // regularization
        {
            if( !diag ) // diagonal entry needs to be inserted
            {
                res.insertBack(P_cols + sub_row, P_cols + sub_row) = -eps;
            }
            else
            {
//                *diag = std::min(-eps, *diag); // adding regularization only on null diagonal is not always enough (constraints can be linearly dependent)
                *diag -= eps; // regularization must be added to every constraint lines
            }
        }
    }

    res.finalize();
}


// note: i removed the non-projected one because it was the *exact
// same* as the projected one.

void SubKKT::projected_primal(SubKKT& res, const AssembledSystem& sys) {
    scoped::timer step("subsystem projection");
    
    // matrix P conveniently filters out fixed dofs
    projection_basis(res.P, sys.P, sys.isPIdentity);
    filter_primal(res.A, sys.H, res.P, sys.isPIdentity);

    // note: i don't care about matrices beeing copied (yet), i want
    // clean code first.
    
    res.Q = rmat();
}




void SubKKT::projected_kkt(SubKKT& res, const AssembledSystem& sys, real eps, bool only_lower)
{
    scoped::timer step("subsystem projection");

    projection_basis(res.P, sys.P, sys.isPIdentity);

    if(sys.n)
    {
        res.Q.resize(sys.n, sys.n);
        res.Q.setIdentity();
        filter_kkt(res.A, sys.H, res.P, res.Q, sys.J, sys.C, eps, sys.isPIdentity, true, only_lower);
    }
    else // no constraints
    {
        filter_primal(res.A, sys.H, res.P);
        res.Q = rmat();
    }
}




SubKKT::SubKKT() {}


void SubKKT::prod(vec& res, const vec& rhs) const {
    res.resize( size_full() );

    vtmp1.resize( size_sub() );
    vtmp2.resize( size_sub() );    

    if( P.cols() ) {
        vtmp1.head(P.cols()).noalias() = P.transpose() * rhs.head(P.rows());
    }
    if( Q.cols() ) {
        vtmp1.tail(Q.cols()).noalias() = Q.transpose() * rhs.tail(Q.rows());
    }

    vtmp2.noalias() = A * vtmp1;

    // remap
    if( P.cols() ) {
        res.head(P.rows()).noalias() = P * vtmp2.head(P.cols());
    }
    
    if( Q.cols() ) {
        res.tail(Q.rows()).noalias() = Q * vtmp2.tail(Q.cols());
    }

}



unsigned SubKKT::size_full() const {
    return P.rows() + Q.rows();
}

unsigned SubKKT::size_sub() const {
    return P.cols() + Q.cols();
}


void SubKKT::solve(const Response& resp,
                   cmat& res,
                   const cmat& rhs) const {
    res.resize(rhs.rows(), rhs.cols());
    
    if( Q.cols() ) {
        throw std::logic_error("sorry, not implemented");
    }

    mtmpc1 = P.transpose() * rhs;

        
    resp.solve(mtmpc2, mtmpc1);

    // mtmp3 = P;

    if( Q.cols() ) {
        res = P * mtmpc2 * Q.transpose();
    }
    else
    {
        // not sure if this causes a temporary
        res = P * mtmpc2;
    }
}

void SubKKT::solve_opt(const Response& resp,
                       cmat& res,
                       const rmat& rhs) const {
    
    if( Q.cols() ) {
        throw std::logic_error("sorry, not implemented");
    }

//    sparse::fast_prod(mtmp1, P.transpose(), rhs.transpose());
//    // mtmp1 = P.transpose() * rhs.transpose();
    
//    resp.solve(mtmp2, mtmp1);
//    mtmp3 = P;

//    // not sure if this causes a temporary
//    sparse::fast_prod(res, mtmp3, mtmp2);
//    // res = mtmp3 * mtmp2;


    mtmpr.resize( rhs.rows(), P.cols() );
    sparse::fast_prod(mtmpr, rhs, P);

    mtmpc2.resize( P.cols(), rhs.rows() );
    resp.solve(mtmpc2, mtmpr.transpose() );

    res.resize(rhs.rows(), rhs.cols());
    mtmpc1 = P;
    sparse::fast_prod(res, mtmpc1, mtmpc2);
}




}
}
}
