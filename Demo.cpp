/*
This file is part of BGSLibrary.

BGSLibrary is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

BGSLibrary is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with BGSLibrary.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <package_tracking/BlobTracking.h>
#include <package_analysis/VehicleCouting.h>


#include "package_bgs/FrameDifferenceBGS.h"
#include "package_bgs/StaticFrameDifferenceBGS.h"
#include "package_bgs/WeightedMovingMeanBGS.h"
#include "package_bgs/WeightedMovingVarianceBGS.h"
#include "package_bgs/MixtureOfGaussianV1BGS.h"
#include "package_bgs/MixtureOfGaussianV2BGS.h"
#include "package_bgs/AdaptiveBackgroundLearning.h"
#include "package_bgs/AdaptiveSelectiveBackgroundLearning.h"

#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >= 4 && CV_SUBMINOR_VERSION >= 3

#include "package_bgs/GMG.h"

#endif

#include "package_bgs/dp/DPAdaptiveMedianBGS.h"
#include "package_bgs/dp/DPGrimsonGMMBGS.h"
#include "package_bgs/dp/DPZivkovicAGMMBGS.h"
#include "package_bgs/dp/DPMeanBGS.h"
#include "package_bgs/dp/DPWrenGABGS.h"
#include "package_bgs/dp/DPPratiMediodBGS.h"
#include "package_bgs/dp/DPEigenbackgroundBGS.h"
#include "package_bgs/dp/DPTextureBGS.h"

#include "package_bgs/tb/T2FGMM_UM.h"
#include "package_bgs/tb/T2FGMM_UV.h"
#include "package_bgs/tb/T2FMRF_UM.h"
#include "package_bgs/tb/T2FMRF_UV.h"
#include "package_bgs/tb/FuzzySugenoIntegral.h"
#include "package_bgs/tb/FuzzyChoquetIntegral.h"

#include "package_bgs/lb/LBSimpleGaussian.h"
#include "package_bgs/lb/LBFuzzyGaussian.h"
#include "package_bgs/lb/LBMixtureOfGaussians.h"
#include "package_bgs/lb/LBAdaptiveSOM.h"
#include "package_bgs/lb/LBFuzzyAdaptiveSOM.h"

#include "package_bgs/ck/LbpMrf.h"
#include "package_bgs/jmo/MultiLayerBGS.h"
// The PBAS algorithm was removed from BGSLibrary because it is
// based on patented algorithm ViBE
// http://www2.ulg.ac.be/telecom/research/vibe/
//#include "package_bgs/pt/PixelBasedAdaptiveSegmenter.h"
#include "package_bgs/av/VuMeter.h"
#include "package_bgs/ae/KDE.h"
#include "package_bgs/db/IndependentMultimodalBGS.h"
#include "package_bgs/sjn/SJN_MultiCueBGS.h"
#include "package_bgs/bl/SigmaDeltaBGS.h"

#include "package_bgs/pl/SuBSENSE.h"
#include "package_bgs/pl/LOBSTER.h"

int main(int argc, char **argv) {
    std::cout << "Using OpenCV " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << "." << CV_SUBMINOR_VERSION
              << std::endl;

    CvCapture *capture = 0;
    int resize_factor = 100;

    if (argc > 1) {
        std::cout << "Openning: " << argv[1] << std::endl;
        capture = cvCaptureFromAVI(argv[1]);
    } else {
        capture = cvCaptureFromCAM(0);
        resize_factor = 50; // set size = 50% of original image
    }

    if (!capture) {
        std::cerr << "Cannot initialize video!" << std::endl;
        return -1;
    }

    IplImage *frame_aux = cvQueryFrame(capture);
    IplImage *frame = cvCreateImage(
            cvSize((int) ((frame_aux->width * resize_factor) / 100), (int) ((frame_aux->height * resize_factor) / 100)),
            frame_aux->depth, frame_aux->nChannels);
    cvResize(frame_aux, frame);

    /* Background Subtraction Methods */
    IBGS *bgs;

    /*** Default Package ***/
    //bgs = new FrameDifferenceBGS;//проверить
    //bgs = new StaticFrameDifferenceBGS;
    //bgs = new WeightedMovingMeanBGS;
    //bgs = new WeightedMovingVarianceBGS;
    //bgs = new MixtureOfGaussianV1BGS;
    //bgs = new MixtureOfGaussianV2BGS;
    bgs = new AdaptiveBackgroundLearning;//проверить
    //bgs = new AdaptiveSelectiveBackgroundLearning;//проверить
    //bgs = new GMG;//проверить

    /*** DP Package (thanks to Donovan Parks) ***/
    //bgs = new DPAdaptiveMedianBGS;
    //bgs = new DPGrimsonGMMBGS;
    //bgs = new DPZivkovicAGMMBGS;
    //bgs = new DPMeanBGS;
    //bgs = new DPWrenGABGS;//достаточно не плохо
    //bgs = new DPPratiMediodBGS;//тоже не плохо
    //bgs = new DPEigenbackgroundBGS; //слишко чувствительно
    //bgs = new DPTextureBGS;//тяжеловесный

    /*** TB Package (thanks to Thierry Bouwmans, Fida EL BAF and Zhenjie Zhao) ***/
    //bgs = new T2FGMM_UM;
    //bgs = new T2FGMM_UV;//слишком чувствительно
    //bgs = new T2FMRF_UM;
    //bgs = new T2FMRF_UV; //не плохо
    //bgs = new FuzzySugenoIntegral;
    //bgs = new FuzzyChoquetIntegral;

    /*** JMO Package (thanks to Jean-Marc Odobez) ***/
    //bgs = new MultiLayerBGS;

    /*** PT Package (thanks to Martin Hofmann, Philipp Tiefenbacher and Gerhard Rigoll) ***/
    //bgs = new PixelBasedAdaptiveSegmenter;

    /*** LB Package (thanks to Laurence Bender) ***/
    //bgs = new LBSimpleGaussian;
    //bgs = new LBFuzzyGaussian;
    //bgs = new LBMixtureOfGaussians;//хорошо
    //bgs = new LBAdaptiveSOM;
    //bgs = new LBFuzzyAdaptiveSOM;//попробовать

    /*** LBP-MRF Package (thanks to Csaba Kertész) ***/
    //bgs = new LbpMrf;

    /*** AV Package (thanks to Lionel Robinault and Antoine Vacavant) ***/
    //bgs = new VuMeter;

    /*** EG Package (thanks to Ahmed Elgammal) ***/
    //bgs = new KDE;

    /*** DB Package (thanks to Domenico Daniele Bloisi) ***/
    //bgs = new IndependentMultimodalBGS;//проверить

    /*** SJN Package (thanks to SeungJong Noh) ***/
    //bgs = new SJN_MultiCueBGS;

    /*** BL Package (thanks to Benjamin Laugraud) ***/
    //bgs = new SigmaDeltaBGS;//проверить

    /*** PL Package (thanks to Pierre-Luc) ***/
    //bgs = new SuBSENSEBGS();
    //bgs = new LOBSTERBGS();



    cv::Mat img_blob;
    BlobTracking *blobTracking;
    blobTracking = new BlobTracking;

    /* Vehicle Counting Algorithm */
    VehicleCouting *vehicleCouting;
    vehicleCouting = new VehicleCouting;

    int key = 0;
    while (key != 'q') {
        frame_aux = cvQueryFrame(capture);
        if (!frame_aux) break;

        cvResize(frame_aux, frame);

        cv::Mat img_input(frame);
        cv::imshow("input", img_input);

        cv::Mat img_mask;
        cv::Mat img_bkgmodel;
        bgs->process(img_input, img_mask, img_bkgmodel); // by default, it shows automatically the foreground mask image

        if (!img_mask.empty()) {
            // Perform blob tracking
            blobTracking->process(img_input, img_mask, img_blob);

            // Perform vehicle counting
            vehicleCouting->setInput(img_blob);
            vehicleCouting->setTracks(blobTracking->getTracks());
            vehicleCouting->process();
        }

        key = cvWaitKey(33);
    }

    delete bgs;

    cvDestroyAllWindows();
    cvReleaseCapture(&capture);

    return 0;
}
