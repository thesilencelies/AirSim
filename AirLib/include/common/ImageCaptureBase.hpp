// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_ImageCaptureBase_hpp
#define air_ImageCaptureBase_hpp

#include "common/Common.hpp"
#include "common/common_utils/EnumFlags.hpp"
#include "common/common_utils/Utils.hpp"

namespace msr
{
namespace airlib
{

    // This is an abstraction for cameras associated with a vehicle.  Each camera has a unique id.
    class ImageCaptureBase
    {
    public: //types
        enum class ImageType : int
        { //this indexes to array, -1 is special to indicate main camera
            Scene = 0,
            DepthPlanar,
            DepthPerspective,
            DepthVis,
            DisparityNormalized,
            Segmentation,
            SurfaceNormals,
            Infrared,
            OpticalFlow,
            OpticalFlowVis,
            FisheyeScene,
            CubeScene,
            CubeDepth,
            Count //must be last
        };

    static bool isCubeType( const ImageType& type ) {
        return type >= ImageType::CubeScene;
    }

    static bool isCubeType( int type ) {
        return isCubeType( common_utils::Utils::toEnum<ImageType>(type) );
    }

    static int getCubeTypeIndex( const ImageType& type ) {
        assert( isCubeType(type) );
        return common_utils::Utils::toNumeric(type) - 
               common_utils::Utils::toNumeric(ImageType::CubeScene);
    }

    static int getCubeTypeIndex( int type ) {
        return getCubeTypeIndex( common_utils::Utils::toEnum<ImageType>(type) );
    }

        struct ImageRequest
        {
            std::string camera_name;
            ImageCaptureBase::ImageType image_type = ImageCaptureBase::ImageType::Scene;
            bool pixels_as_float = false;
            bool compress = true;

            ImageRequest()
            {
            }

            ImageRequest(const std::string& camera_name_val,
                         ImageCaptureBase::ImageType image_type_val,
                         bool pixels_as_float_val = false,
                         bool compress_val = true)
                : camera_name(camera_name_val)
                , image_type(image_type_val)
                , pixels_as_float(pixels_as_float_val)
                , compress(compress_val)
            {
            }
        };

        struct ImageResponse
        {
            vector<uint8_t> image_data_uint8;
            vector<float> image_data_float;

            std::string camera_name;
            Vector3r camera_position = Vector3r::Zero();
            Quaternionr camera_orientation = Quaternionr::Identity();
            TTimePoint time_stamp = 0;
            std::string message;
            bool pixels_as_float = false;
            bool compress = true;
            int width = 0, height = 0;
            ImageType image_type;
        };

    public: //methods
        virtual void getImages(const std::vector<ImageRequest>& requests, std::vector<ImageResponse>& responses) const = 0;
        virtual ~ImageCaptureBase() = default;
    };
}
} //namespace
#endif
