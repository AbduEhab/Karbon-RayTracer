#pragma once

#include "Constants.hpp"
#include "Tuples/Color.hpp"
#include "json.hpp"
#include "stb_image_write.h"

namespace Karbon
{

    [[nodiscard]] std::string read_file(const std::string &filepath)
    {
        PROFILE_FUNCTION();

        debug_print(std::string("Reading file: ") + filepath);

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);

        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = (size_t)in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                // ToDO: handle failure to read from file
                debug_print(-1);
            }
        }
        else
        {
            // ToDO: handle failure to open file
            debug_print(-2);
        }

        return result;
    }

    int write_file(const std::string &filepath, const std::string &message)
    {
        PROFILE_FUNCTION();

        std::ofstream out(filepath, std::ios::out | std::ios::binary);

        if (out)
        {
            if (message.size())
            {
                out << message << std::endl;

                out.close();
            }
            else
            { // handle empty string case

                debug_print("failed to Write to file (cannot write empty string)");
                return -1;
            }
        }
        else
        { // handle failure to open file

            debug_print("failed to Write to file (cannot open file)");
            return -2;
        }

        return 1;
    }

    [[nodiscard]] std::string get_file_extension(const std::string &filepath)
    {
        PROFILE_FUNCTION();

        std::string extension;

        size_t last_dot = filepath.find_last_of('.');
        if (last_dot != std::string::npos)
        {
            extension = filepath.substr(last_dot + 1);
        }

        return extension;
    }

    int save_image(std::shared_ptr<Karbon::Color[]> canvas, int width, int height, std::string filename = "ExampleRender.jpg")
    {
        PROFILE_FUNCTION();

        debug_print("Saving image");

        uint8_t *pixels = new uint8_t[width * height * 3];

        int index = 0;

        for (int32_t i = 0; i < width * height; i++)
        {
            Karbon::Color curr = canvas[i];

            pixels[index++] = (uint8_t)curr.r;
            pixels[index++] = (uint8_t)curr.g;
            pixels[index++] = (uint8_t)curr.b;
        }

        // You have to use 3 comp for complete jpg file. If not, the image will be grayscale or nothing.
        int result = stbi_write_jpg(filename.c_str(), width, height, 3, pixels, 100);
        delete[] pixels;

        if (result == 0)
        {
            debug_print("failed to save image");
            return -1;
        }

        debug_print("image saved");
        return 1;
    }

}
