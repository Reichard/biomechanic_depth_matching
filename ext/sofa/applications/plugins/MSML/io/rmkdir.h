/*
 * rmkdir.h
 *
 *  Created on: Apr 2, 2013
 *      Author: steverma
 */

#ifndef RMKDIR_H_
#define RMKDIR_H_

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * r[ecursively]m[a]k[e]dir[ectories] emulates the behaviour of the mkdir -p shell
 * command, that is, it creates as many (parent) directories as necessary.
 *
 * rmkdirfor(filename) encapsulates rmkdir(dirname) and therefore creates all
 * the parent directories of the given path, but does not interpret the path
 * itself as a directory.
 */
inline int rmkdir(const char *dir) {
	using std::cerr;
	using std::endl;
	struct stat temp;
	if (!stat(dir, &temp)) return 0;
	switch (errno) {
		case 0:
			assert(false);
			break;
		case ENOENT:
			break;
		default:
			int saved_errno = errno;
			cerr << "rmkdir: stat(" << dir << "failed with error "
				<< saved_errno << ": " << strerror(saved_errno) << endl;
			return saved_errno;
	}
	char *copy = strdup(dir);
	if (copy == NULL) {
		cerr << "rmkdir: could not allocate memory" << endl;
		return ENOMEM;
	}
	char *ancestor = dirname(copy);
	if (copy == NULL) {
		cerr << "rmkdir: dirname failed" << endl;
		return -1;
	}
	int ret = rmkdir(ancestor);
	free(copy);
	if (ret) return ret;
	if (!mkdir(dir, 0755)) return 0;
	int saved_errno = errno;
	cerr << "rmkdir: mkdir(" << dir << ") failed with error " << saved_errno
		<< ": " << strerror(saved_errno) << endl;
	return saved_errno;
}

inline int rmkdir(const std::string& dir) {
	return rmkdir(dir.c_str());
}

inline int rmkdirfor(const std::string& dir) {
	char *temp = strdup(dir.c_str());
	if (temp == NULL) {
		std::cerr << "rmkdirfor: could not allocate memory" << std::endl;
		return ENOMEM;
	}
	int result = rmkdir(dirname(temp));
	free(temp);
	return result;
}

inline int rmkdirfor(const char *dir) {
	char *temp = strdup(dir);
	int result = rmkdir(dirname(temp));
	free(temp);
	return result;
}

#endif /* RMKDIR_H_ */
