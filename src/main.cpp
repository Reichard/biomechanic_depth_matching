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
	int substeps = 1;
	float dt = 4.0f;
};

class DepthMatch {
	public:
		DepthMatch(DepthMatchConfig conf) : config(conf) {
			std::vector<std::string> plugins = {
				"./sofa-prefix/src/sofa-build/lib/libSofaCUDA.so",
				"./src/libDepthMatch.so"
			};
			sofa_scene.loadPlugins(plugins);
			//scene_root = make_scene();
			//sofa_scene.setScene(&*scene_root);
			sofa_scene.open(DATA_DIR"/scenes/liver_deform.scn");
		}

		void update(const mediassist::rgbd_image &image) {
			for(int substep=0; substep<config.substeps; ++substep){
				sofa_scene.step(config.dt);
			}
		}

		sofa::simplegui::SofaScene &scene() {
			return sofa_scene;
		}
	private:
		DepthMatchConfig config;
		sofa::simplegui::SofaScene sofa_scene;
		sofa::simulation::Node::SPtr scene_root;
};

int main() {
	DepthMatchConfig config; 

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
