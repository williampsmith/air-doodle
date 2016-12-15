#include <GRT/GRT.h>
#include <sstream>
#include <string>

using namespace GRT;
using namespace std;

int main (int argc, const char * argv[])
{
    if (argc < 6) {
        cout << "Not enough arguments! Only got " << argc-1 << " expected 3" << endl;
        return EXIT_FAILURE;
    }

    int numGestures = strtol(argv[2], NULL, 10);
    int numSamples = strtol(argv[3], NULL, 10);
    int numCols = strtol(argv[4], NULL, 10);

    //Create a new instance of the TimeSeriesClassificationData
    TimeSeriesClassificationData trainingData;

    //Set the dimensionality of the data (you need to do this before you can add any samples)
    trainingData.setNumDimensions(numCols);

    //You can also give the dataset a name (the name should have no spaces)
    trainingData.setDatasetName(argv[1]);

    //You can also add some info text about the data
    trainingData.setInfoText("This data contains the initial training samples collected, in the format x_orientation, y_orientation, z_orientation, x_accel, y_accel, z_accel.");

    //Here you would record a time series, when you have finished recording the time series then add the training sample to the training data
    // Classification label --> gesture mapping: 1 --> 0, 2 --> 8, 3 --> 9
    UINT gestureLabel = 1;
    MatrixDouble trainingSample;
    VectorDouble sample(numCols);

    ifstream infile(argv[5]);

    string line;
    for (int i = 0; i < numGestures; i++) { // for each classification index (0, 8 and 9 in our case)
      gestureLabel = i + 1;

      for (int j = 0; j < numSamples; j++) {
        trainingSample.clear();
        cout << endl << "Clearing training sample matrix. Size is " << trainingSample.getSize() << endl << endl;

        getline(infile, line);
        cout << "Line retrieved: " << line << endl;

        while (getline(infile, line)) {
          cout << "Line retrieved: " << line << endl;
          if ((line[0] != '%') && line.length() > 1){ // check for empty lines and delimiter lines
            cout << "Creating new training sample." << endl;
            istringstream iss(line);

            // populate the sample vector
            for (int k = 0; k < numCols; k++) {
                iss >> sample[k];
            }

            trainingSample.push_back(sample);
            cout << "training sample size: " << trainingSample.getSize() << endl;
          }
          else { // reached end of sample
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
    if( !trainingData.saveDatasetToFile(argv[6]) ){
        cout << "Failed to save dataset to file!\n";
        return EXIT_FAILURE;
    }

    return 0;
  }
