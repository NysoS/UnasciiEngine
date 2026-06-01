#include "Exar/ImageView.hpp"
#include "Exar/Internal/ResourceTypes.h"

Exar::Image Exar::ImageView::getImage() const
{
    return mDesc.image;
}

const Exar::u8* Exar::ImageView::getData() const
{
    Image_* lImage = static_cast<Image_*>(mDesc.image);
    
    if (!lImage) return nullptr;

    return static_cast<u8*>(lImage->data);
}
