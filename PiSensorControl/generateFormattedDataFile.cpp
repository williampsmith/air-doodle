#include <GRT/GRT.h>
#include <sstream>
#include <string>

using namespace GRT;
using namespace std;

int main (int argc, const char * argv[])
{
    if (argc < 4) {
        cout << "Not enough arguments! Only got " << argc-1 << " expected 3" << endl;
        return EXIT_FAILURE;
    }

    //Create a new instance of the TimeSeriesClassificationData
    TimeSeriesClassificationData trainingData;

    //Set the dimensionality of the data (you need to do this before you can add any samples)
    trainingData.setNumDimensions( 5 );

    //You can also give the dataset a name (the name should have no spaces)
    trainingData.setDatasetName(argv[1]);

    //You can also add some info text about the data
    trainingData.setInfoText("This data contains the initial training samples collected, in the format x_orientation, y_orientation, z_orientation, x_accel, y_accel, z_accel.");

    //Here you would record a time series, when you have finished recording the time series then add the training sample to the training data
    // Classification label --> gesture mapping: 1 --> 0, 2 --> 8, 3 --> 9
    UINT gestureLabel = 1;
    MatrixDouble trainingSample;

    ifstream infile(argv[2]);

    string line;
    for (int i = 0; i < 3; i++) { // for each classification index (0, 8 and 9 in our case)
      gestureLabel = i + 1;

      for (int j = 0; j < 20; j++) {
        trainingSample.clear();
        cout << endl << "Clearing training sample matrix. Size is " << trainingSample.getSize() << endl << endl;

        getline(infile, line);
        //cout << "Line retrieved: " << line << endl;

        while (getline(infile, line)) {
          cout << "Line retrieved: " << line << endl;
          if ((line[0] != '%') && line.length() > 3){ // check for empty lines and delimiter lines
            //cout << "Creating new training sample." << endl;
            VectorDouble sample( 5 );
            istringstream iss(line);
            double x_or, y_or, z_or, x_accel, y_accel, z_accel;

            // get all the data points from the line
            iss >> x_or >> y_or >> z_or >> x_accel >> y_accel >> z_accel;
            cout << "Sample:" << endl;
            cout /*<< x_or << "  " */ << y_or << "  " << z_or << "  " << x_accel << "  " << y_accel << "  " << z_accel << endl;

            // populate the sample vector
            //sample[0] = x_or;
            sample[0] = y_or;
            sample[1] = z_or;
            sample[2] = x_accel;
            sample[3] = y_accel;
            sample[4] = z_accel;

            trainingSample.push_back(sample);
            cout << "training sample size: " << trainingSample.getSize() << endl;
          }
          else if ((line.length() < 2) || ((line[0] != '%') && (line[1] != '%'))) { // reached end of sample
            cout << "\n\n----------- NEW SAMPLE ----------------\n\n";
            break;
          }
        }

        //Add the training sample to the dataset
        if (trainingSample.getSize() > 0)
          trainingData.addSample( gestureLabel, trainingSample );
      }
    }

    //After recording your training data you can then save it to a file
    if( !trainingData.saveDatasetToFile(argv[3]) ){
        cout << "Failed to save dataset to file!\n";
        return EXIT_FAILURE;
    }

    return 0;
  }
