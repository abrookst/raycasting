#ifndef TEXTURE_H
#define TEXTURE_H

struct Texture {
    size_t img_w, img_h;
    size_t count, size;
    std::vector<uint32_t> img;

    Texture(const std::string filename);
    uint32_t& get(const size_t i, const size_t j, const size_t idx); // get the pixel (i,j) from the textrue idx
    std::vector<uint32_t> get_scaled_column(const size_t texture_id, const size_t tex_coord, const size_t column_height); // retrieve one column (tex_coord) from the texture texture_id and scale it to the destination size
};

#endif