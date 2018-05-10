#include "MaxRectsBinPack.h"
#include "Rect.h"
#include <plotter.h>
#include <cstdio>
#include <fstream>
#include <algorithm>

using namespace rbp;
struct subarr {
    RectSize recsz;
    char arrType;
//    subarr(Rectsize & rs, char atype) : recsz{rs}, arrType{atype} {};
};

int DrawMatLayout(std::vector<Rect> & packedRects, rbp::RectSize & decoder){
    // set a Plotter parameter
    PlotterParams params;
    params.setplparam ("PAGESIZE", (char *)"letter,xorigin=2in,yorigin=3in");

    ofstream outputfile;
    outputfile.open("matlayout_121.ps");
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

int main()
{
    subarr SRAM{{10, 15}, 'S'};
    subarr eDRAM{{4, 5}, 'D'};
    subarr NVM{{3, 3}, 'N'};

    rbp::RectSize decoder{20, 40};

    int numSRAM = 64;
    int numeDRAM = 128;
    int numNVM = 64;

    std::vector<RectSize> inputRects(numSRAM+numeDRAM+numNVM);
    std::vector<Rect> packedRects(numSRAM+numeDRAM+numNVM);
    std::fill_n(inputRects.begin(), numSRAM, SRAM.recsz);
    std::fill_n(inputRects.begin() + numSRAM, numeDRAM, eDRAM.recsz);
    std::fill_n(inputRects.begin() + numSRAM + numeDRAM, numNVM, NVM.recsz);

    // Create a bin to pack to, use the bin size from command line.
    int binWidth = SRAM.recsz.width * 11;
    int binHeight = SRAM.recsz.height * 8;
    MaxRectsBinPack bin(binWidth, binHeight, false);
    printf("Initializing bin to size %dx%d.\n", binWidth, binHeight);

    // Perform the packing.
    MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = MaxRectsBinPack::RectBestAreaFit; // This can be changed individually even for each rectangle packed.
    bin.Insert(inputRects, packedRects, heuristic);

    // Test success or failure.
    printf("Done. All rectangles packed. Free space left: %.2f%%\n", 100.f - bin.Occupancy()*100.f);
    return DrawMatLayout(packedRects, decoder);
}

