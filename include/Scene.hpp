#include <Constants.hpp>

namespace Karbon
{
    struct Scene
    {
        [[nodiscard]] Scene() = default;
        [[nodiscard]] Scene(Camera c, World w = World(0)) : m_world(w), m_camera(c) {}

        // save scene to file as json
        void save_scene(const std::string &file_name) const
        {
            PROFILE_FUNCTION();

            // create json object
            nlohmann::json json;

            // add camera to json
            json["camera"] = nlohmann::json::parse(m_camera.to_json());

            // add world to json
            json["world"] = nlohmann::json::parse(m_world.to_json());

            // write json to file
            std::ofstream file(file_name);
            file << json.dump(4);
            file.close();
        }

        // load scene from file as json
        void load_scene(const std::string &file_name)
        {
            PROFILE_FUNCTION();

            // read json from file
            std::ifstream file(file_name);
            nlohmann::json json;
            file >> json;

            // get camera from json
            m_camera.from_json(json["camera"].dump());

            // get world from json
            m_world.from_json(json["world"].dump());
            
        }

        World m_world;
        Camera m_camera;
    };
}