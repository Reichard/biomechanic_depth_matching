#include "SofaSimpleGUI/SofaScene.h"
#include "SofaSimpleGUI/SofaGL.h"
#include "SofaSimpleGUI/Camera.h"
#include <vector>
#include <string>
#include <memory>

#include "scene.hpp"
#include "rgbd_source.hpp"
#include "rgbd_image.hpp"
#include "viewer.hpp"

struct DepthMatchConfig {
	std::string scene_file = "";
	float dt = 0.04f;
};

class DepthMatch {
	public:
		DepthMatch(DepthMatchConfig conf) : config(conf) {
			std::vector<std::string> plugins = {
				"./sofa-prefix/src/sofa-build/lib/libSofaCUDA.so",
				"./lib/libDepthMatch.so"
			};
			sofa_scene.loadPlugins(plugins);
			sofa_scene.open(config.scene_file);
		}

		void update(const mediassist::rgbd_image &image) {
			sofa_scene.step(config.dt);
		}

		sofa::simplegui::SofaScene &scene() {
			return sofa_scene;
		}
	private:
		DepthMatchConfig config;
		sofa::simplegui::SofaScene sofa_scene;
		sofa::simulation::Node::SPtr scene_root;
};

int main(int argc, char* argv[]) {
	if(argc != 2) {
		std::cout << "Usage: " << argv[0] << " SCENE_FILE" << std::endl;
		return 1;
	}

	DepthMatchConfig config; 
	config.scene_file = argv[1];

	Viewer viewer;
	DepthMatch tracker(config);
	viewer.set_scene(tracker.scene());

	mediassist::RgbdSource rgbd_source;

	for(int frame=0;frame<MAX_FRAMES;++frame) {
		auto image = rgbd_source.read(frame);
		rgbd_source.prefetch(frame+1);

		tracker.update(image);
		viewer.update();
	}
}
