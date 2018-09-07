/*
 * csi.h
 *
 *  Created on: Sep 3, 2018
 *      Author: LeeChunHei
 */

#ifndef INC_DRIVER_CSI_H_
#define INC_DRIVER_CSI_H_

#include <stdint.h>

namespace Driver{

#define FSL_VIDEO_FOURCC(a, b, c, d)	((uint32_t)(a) | ((uint32_t)(b) << 8U) | ((uint32_t)(c) << 16U) | ((uint32_t)(d) << 24U))

class CSI{
public:
	struct Config{
		enum struct PixelFormat{
		    /* RGB */
		    kXRGB8888 = FSL_VIDEO_FOURCC('X', 'R', '2', '4'), /*!< 32-bit XRGB8888. */
		    kRGBX8888 = FSL_VIDEO_FOURCC('R', 'X', '2', '4'), /*!< 32-bit RGBX8888. */
		    kXBGR8888 = FSL_VIDEO_FOURCC('X', 'B', '2', '4'), /*!< 32-bit XBGR8888. */
		    kBGRX8888 = FSL_VIDEO_FOURCC('B', 'X', '2', '4'), /*!< 32-bit BGRX8888. */

		    kRGB888 = FSL_VIDEO_FOURCC('R', 'G', '2', '4'), /*!< 24-bit RGB888. */
		    kBGR888 = FSL_VIDEO_FOURCC('B', 'G', '2', '4'), /*!< 24-bit BGR888. */

		    kRGB565 = FSL_VIDEO_FOURCC('R', 'G', '1', '6'), /*!< 16-bit RGB565. */
		    kBGR565 = FSL_VIDEO_FOURCC('B', 'G', '1', '6'), /*!< 16-bit BGR565. */

		    kXRGB1555 = FSL_VIDEO_FOURCC('X', 'R', '1', '5'), /*!< 16-bit XRGB1555. */
		    kRGBX5551 = FSL_VIDEO_FOURCC('R', 'X', '1', '5'), /*!< 16-bit RGBX5551. */
		    kXBGR1555 = FSL_VIDEO_FOURCC('X', 'B', '1', '5'), /*!< 16-bit XBGR1555. */
		    kBGRX5551 = FSL_VIDEO_FOURCC('B', 'X', '1', '5'), /*!< 16-bit BGRX5551. */

		    kXRGB4444 = FSL_VIDEO_FOURCC('X', 'R', '1', '2'), /*!< 16-bit XRGB4444. */
		    kRGBX4444 = FSL_VIDEO_FOURCC('R', 'X', '1', '2'), /*!< 16-bit RGBX4444. */
		    kXBGR4444 = FSL_VIDEO_FOURCC('X', 'B', '1', '2'), /*!< 16-bit XBGR4444. */
		    kBGRX4444 = FSL_VIDEO_FOURCC('B', 'X', '1', '2'), /*!< 16-bit BGRX4444. */

		    /* YUV. */
		    kYUYV = FSL_VIDEO_FOURCC('Y', 'U', 'Y', 'V'), /*!< YUV422, Y-U-Y-V. */
		    kYVYU = FSL_VIDEO_FOURCC('Y', 'V', 'Y', 'U'), /*!< YUV422, Y-V-Y-U. */
		    kUYVY = FSL_VIDEO_FOURCC('U', 'Y', 'V', 'Y'), /*!< YUV422, U-Y-V-Y. */
		    kVYUY = FSL_VIDEO_FOURCC('V', 'Y', 'U', 'Y'), /*!< YUV422, V-Y-U-Y. */

		    kXYUV = FSL_VIDEO_FOURCC('X', 'Y', 'U', 'V'), /*!< YUV444, X-Y-U-V. */
		    kXYVU = FSL_VIDEO_FOURCC('X', 'Y', 'V', 'U'), /*!< YUV444, X-Y-V-U. */
		};
		enum struct Interface{
		    kGatedClock,    /*!< HSYNC/HREF, VSYNC, and PIXCLK signals are used. */
		    kNonGatedClock, /*!< VSYNC, and PIXCLK signals are used. */
		    kCCIR656,       /*!< CCIR656 interface. */
		    kMIPI,          /*!< MIPI CSI2 interface. */
		};
	    PixelFormat pixel_format;    /*!< Pixel format. */
	    uint8_t bytesPerPixel;               /*!< Byte per pixel. */
	    uint32_t resolution;                 /*!< Resolution, see @ref video_resolution_t and @ref FSL_VIDEO_RESOLUTION. */
	    uint16_t frameBufferLinePitch_Bytes; /*!< Frame buffer line pitch in bytes. */
	    Interface interface;        /*!< Interface type. */
	    uint32_t controlFlags;               /*!< Control flags, OR'ed value of @ref _camera_flags. */
	    uint8_t framePerSec;                 /*!< Frame per second. */
	    uint8_t mipiChannel;                 /*!< MIPI virtual channel. */
	    uint8_t csiLanes;                    /*!< MIPI CSI data lanes number. */
	};
	CSI(Config& config);
};

}

#endif /* INC_DRIVER_CSI_H_ */
