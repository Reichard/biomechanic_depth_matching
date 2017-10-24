#pragma once

#include <vector>
#include <fstream>

struct PerFrameStats
{
	float surface_rmsd = 0;
	float avg_surface_distance = 0;

	float volume_rmsd = 0;
	float avg_volume_distance = 0;

	float projective_error_surface_to_ground_truth = 0;
	float projective_error_surface_to_depth = 0;
	float projective_error_depth_to_ground_truth = 0;
};

struct Stats
{
	int substeps = 0;
	int pixel_step = 0;
	float max_stiffness = 0;
	float max_data_dist = 0;
	std::vector<PerFrameStats> per_frame;

	void reset()
	{
		per_frame.clear();
	}

	PerFrameStats &current()
	{
		return per_frame.back();
	}

	void advance()
	{
		PerFrameStats new_frame_stats;
		per_frame.push_back( new_frame_stats );
	}

	void write(const std::string &filename)
	{
		std::ofstream file(filename);
		
		file << "substeps " << substeps << std::endl;
		file << "pixel_step " << pixel_step << std::endl;
		file << "max_stiffness " << max_stiffness << std::endl;
		file << "max_data_dist " << max_data_dist << std::endl;
		file << "frames " << per_frame.size() << std::endl;
		file << std::endl;

		file << "surface_rmsd" << std::endl;
		for(auto &frame : per_frame)
		{
			file << frame.surface_rmsd << std::endl;
		}
		file << std::endl;

		file << "avg_surface_distance" << std::endl;
		for(auto &frame : per_frame)
		{
			file << frame.avg_surface_distance << std::endl;
		}
		file << std::endl;

		file << "volume_rmsd" << std::endl;
		for(auto &frame : per_frame)
		{
			file << frame.volume_rmsd << std::endl;
		}
		file << std::endl;

		file << "avg_volume_distance" << std::endl;
		for(auto &frame : per_frame)
		{
			file << frame.avg_volume_distance << std::endl;
		}
		file << std::endl;

		file << "projective_error_surface_to_ground_truth" << std::endl;
		for(auto &frame : per_frame)
		{
			file << frame.projective_error_surface_to_ground_truth << std::endl;
		}
		file << std::endl;

		file << "projective_error_depth_to_ground_truth" << std::endl;
		for(auto &frame : per_frame)
		{
			file << frame.projective_error_depth_to_ground_truth << std::endl;
		}
		file << std::endl;
	}
};
