#include "MaxRectsBinPack.h"
#include "Rect.h"
#include <plotter.h>
#include <cstdio>
#include <fstream>
#include <algorithm>
#include <boost/program_options.hpp>

using namespace rbp;
struct subarr {
    RectSize recsz;
    char arrType;
//    subarr(Rectsize & rs, char atype) : recsz{rs}, arrType{atype} {};
};

int DrawMatLayout(std::vector<Rect> & packedRects,
        rbp::RectSize & decoder,
        ofstream &outputfile
        ){
    // set a Plotter parameter
    PlotterParams params;
    params.setplparam ("PAGESIZE", (char *)"letter,xorigin=2in,yorigin=3in");

    PSPlotter plotter(cin, outputfile, cerr, params); // declare Plotter
    if (plotter.openpl () < 0)                  // open Plotter
      {
        cerr << "Couldn't open Plotter\n";
        return 1;
      }

    plotter.fspace (0.0, 0.0, 100.0, 100.0); // specify user coor system
    plotter.flinewidth (0.25);       // line thickness in user coordinates
    plotter.colorname("black");
    plotter.erase ();                // erase Plotter's graphics display

    // Move origin to the center of the coord system range
    double origin_x = 50;
    double origin_y = 50;

    // Plot the Decoder block
    plotter.box(origin_x - decoder.width/2, origin_y - decoder.height/2,
            origin_x + decoder.width/2, origin_y + decoder.height/2);
    plotter.textangle(90);
    plotter.ffontsize(decoder.width/2);
    plotter.alabel('c','c',"Decoder");
    plotter.textangle(0);

    //Plot rectangles
    double offset_x;
    double offset_y;
    char label[2] = "S";
    for (auto & rec : packedRects) {
        if (rec.width == 10){
            plotter.filltype(0xFFFF);
            label[0] = 'S';
        } else if (rec.width == 4) {
            plotter.filltype(0xA000);
            label[0] = 'D';
        } else {
            plotter.filltype(0x6000);
            label[0] = 'N';
        }
        // First Quadrant
        offset_x = origin_x + decoder.width/2;
        offset_y = origin_y + decoder.height/2;
        plotter.fbox(rec.x + offset_x, rec.y + offset_y,
                rec.width + rec.x + offset_x, rec.height + rec.y + offset_y);
        plotter.ffontsize(rec.width/2);
        plotter.alabel('c','c', label);
        // Second Quadrant
        offset_x = origin_x - decoder.width/2;
        offset_y = origin_y + decoder.height/2;
        plotter.fbox(0 - rec.x - rec.width + offset_x, rec.y + offset_y,
                0 - rec.x + offset_x, rec.height + rec.y + offset_y);
        plotter.ffontsize(rec.width/2);
        plotter.alabel('c','c', label);
        // Third Quadrant
        offset_x = origin_x - decoder.width/2;
        offset_y = origin_y - decoder.height/2;
        plotter.fbox(0 - rec.x - rec.width + offset_x,
                0 - rec.y -rec.height + offset_y,
                0 - rec.x + offset_x,
                0 - rec.y + offset_y);
        plotter.ffontsize(rec.width/2);
        plotter.alabel('c','c', label);
        // Forth Quadrant
        offset_x = origin_x + decoder.width/2;
        offset_y = origin_y - decoder.height/2;
        plotter.fbox( rec.x + offset_x, 0 - rec.y - rec.height + offset_y,
                rec.width + rec.x + offset_x, 0 - rec.y + offset_y);
        plotter.ffontsize(rec.width/2);
        plotter.alabel('c','c', label);
    }

    if (plotter.closepl () < 0)      // close Plotter
      {
        cerr << "Couldn't close Plotter\n";
        return 1;
      }
    return 0;

}

int main(int ac, char **av)
{
    // cmdline options
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("numSubarray", po::value<int>(), "number of subarray in one quadrant")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po:notify(vm);
    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
    if (vm.count("numSubarray")) {
        cout << "numSubarray set to " << vm["numSubarray"].as<int>() << ".\n";
    } else {
        cout << "numSubarray was not set.\n";
    }
    // objects & variables declaration
    ofstream outputfile;
    ofstream outputfile_orig;
    subarr SRAM{{10, 15}, 'S'};
    subarr eDRAM{{4, 5}, 'D'};
    subarr NVM{{3, 3}, 'N'};
    rbp::RectSize decoder{20, 40};

    int numSubarray = vm["numSubarray"].as<int>();
    int numSRAM = 2;
    int numeDRAM = 1;
    int numNVM = 1;

    std::vector<RectSize> inputRects(numSRAM+numeDRAM+numNVM);
    std::vector<Rect> packedRects(numSRAM+numeDRAM+numNVM);
    std::vector<Rect> origRects(numSRAM+numeDRAM+numNVM);


    int binWidth = SRAM.recsz.width * 11;
    int binHeight = SRAM.recsz.height * 8;
    MaxRectsBinPack bin(binWidth, binHeight, false);
    MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = MaxRectsBinPack::RectBestAreaFit; // This can be changed individually even for each rectangle packed.

    for (int i = 1; i <= 3; i++)
    {
        switch (i) {
            case 1:
                numSRAM = numSubarray/2;
                numeDRAM = numSubarray/4;
                numNVM = numSubarray/4;
                outputfile.open(std::to_string(numSubarray)+std::string("_211.ps"));
                outputfile_orig.open(std::to_string(numSubarray)+std::string("_211_orig.ps"));
                break;
            case 2:
                numSRAM = numSubarray/4;
                numeDRAM = numSubarray/2;
                numNVM = numSubarray/4;
                outputfile.open(std::to_string(numSubarray)+std::string("_121.ps"));
                outputfile_orig.open(std::to_string(numSubarray)+std::string("_121_orig.ps"));
                break;
            case 3:
                numSRAM = numSubarray/4;
                numeDRAM = numSubarray/4;
                numNVM = numSubarray/2;
                outputfile.open(std::to_string(numSubarray)+std::string("_112.ps"));
                outputfile_orig.open(std::to_string(numSubarray)+std::string("_112_orig.ps"));
                break;
        }

        //Iterating and find the proper x, y that yields the least area.
        int num_x;
        int num_y;
        int x;
        int y;
        double space_left = std::numeric_limits<int>::max();
        int sides_diff = std::numeric_limits<int>::max();
        int bin_area = std::numeric_limits<int>::max();
        for (x = sqrt(numSubarray); x >= sqrt(numSubarray)/2; x--)
        {
            for (y = sqrt(numSubarray); y >= sqrt(numSubarray)/2; y--)
            {
                inputRects.resize(numSRAM+numeDRAM+numNVM);
                packedRects.resize(numSRAM+numeDRAM+numNVM);
                std::fill_n(inputRects.begin(), numSRAM, SRAM.recsz);
                std::fill_n(inputRects.begin() + numSRAM, numeDRAM, eDRAM.recsz);
                std::fill_n(inputRects.begin() + numSRAM + numeDRAM, numNVM, NVM.recsz);
                binWidth = SRAM.recsz.width * (x);
                binHeight = SRAM.recsz.height * (y);
                bin.Init(binWidth, binHeight, false);
                // Perform packing
                if (bin.Insert(inputRects, packedRects, heuristic)) {
                    //if ((100-bin.Occupancy()*100)<space_left) {
                    //    space_left = 100-bin.Occupancy()*100;
                    if ((abs(binWidth*binHeight))<bin_area) {
                        bin_area = binWidth*binHeight;
                        space_left = 100-bin.Occupancy()*100;
                        sides_diff = abs(binWidth-binHeight);
                        num_x = x;
                        num_y = y;
                    }
                }
            }
        }
        printf ("Found area optimized bin.\n");
        binWidth = SRAM.recsz.width * num_x;
        binHeight = SRAM.recsz.height * num_y;
        bin.Init(binWidth, binHeight, false);
        printf("Initializing bin to size %dx%d.\n", binWidth, binHeight);
        // Perform packing
        inputRects.resize(numSRAM+numeDRAM+numNVM);
        packedRects.resize(numSRAM+numeDRAM+numNVM);
        std::fill_n(inputRects.begin(), numSRAM, SRAM.recsz);
        std::fill_n(inputRects.begin() + numSRAM, numeDRAM, eDRAM.recsz);
        std::fill_n(inputRects.begin() + numSRAM + numeDRAM, numNVM, NVM.recsz);
        bin.Insert(inputRects, packedRects, heuristic);
        printf("Done. All rectangles packed. Free space left: %.2f%%\n",
                100.f - bin.Occupancy()*100.f);
        Rect defualt_SRAM_rect{0,0,10,15};
        Rect defualt_eDRAM_rect{0,0,4,5};
        Rect defualt_NVM_rect{0,0,3,3};
        origRects.resize(numSRAM+numeDRAM+numNVM);
        std::fill_n(origRects.begin(), numSubarray, defualt_SRAM_rect);
        std::fill_n(origRects.begin() + numSRAM, numeDRAM, defualt_eDRAM_rect);
        std::fill_n(origRects.begin() + numSRAM + numeDRAM, numNVM, defualt_NVM_rect);
        for (int m = 0; m < std::sqrt(numSubarray); m++)
            for (int n = 0; n < std::sqrt(numSubarray); n++){
                origRects[m*sqrt(numSubarray)+n].y = m * SRAM.recsz.height;
                origRects[m*sqrt(numSubarray)+n].x = n * SRAM.recsz.width;
            }

        DrawMatLayout(packedRects, decoder, outputfile);
        DrawMatLayout(origRects, decoder, outputfile_orig);
        outputfile.close();
        outputfile_orig.close();
    }

    return 0;
}

