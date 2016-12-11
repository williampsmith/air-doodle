#include <GRT/GRT.h>
#include <sstream>
#include <string>

using namespace GRT;
using namespace std;

int main (int argc, const char * argv[])
{
    //Create a new instance of the TimeSeriesClassificationData
    TimeSeriesClassificationData trainingData;

    //Set the dimensionality of the data (you need to do this before you can add any samples)
    trainingData.setNumDimensions( 6 );

    //You can also give the dataset a name (the name should have no spaces)
    trainingData.setDatasetName("initialTrainingData");

    //You can also add some info text about the data
    trainingData.setInfoText("This data contains the initial training samples collected, in the format x_orientation, y_orientation, z_orientation, x_accel, y_accel, z_accel.");

    //Here you would record a time series, when you have finished recording the time series then add the training sample to the training data
    // Classification label --> gesture mapping: 1 --> 0, 2 --> 8, 3 --> 9
    UINT gestureLabel = 1;
    MatrixDouble trainingSample;

    ifstream infile("trainingData/preformattedData.txt");

    string line;
    for (int i = 0; i < 3; i++) { // for each classification index (0, 8 and 9 in our case)
      gestureLabel = i + 1;

      for (int j = 0; j < 20; j++) {
        trainingSample.clear();
        cout << endl << "Clearing training sample matrix. Size is " << trainingSample.getSize() << endl << endl;

        while (getline(infile, line)) {
          if ((line[0] != '%') && line.length() > 3){ // check for empty lines and delimiter lines
            VectorDouble sample( 6 );
            istringstream iss(line);
            double x_or, y_or, z_or, x_accel, y_accel, z_accel;

            // get all the data points from the line
            iss >> x_or >> y_or >> z_or >> x_accel >> y_accel >> z_accel;
            // cout << "Sample:" << endl;
            // cout << x_or << "  " << y_or << "  " << z_or << "  " << x_accel << "  " << y_accel << "  " << z_accel << endl;

            // populate the sample vector
            sample[0] = x_or;
            sample[1] = y_or;
            sample[2] = z_or;
            sample[3] = x_accel;
            sample[4] = y_accel;
            sample[5] = z_accel;

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

    // //For now we will just add 10 x 20 random walk data timeseries
    // Random random;
    // for(UINT k = 0; k < 2; k++) {
    //     gestureLabel = k+1;
    //
    //     //Get the init random walk position for this gesture
    //     VectorDouble startPos( trainingData.getNumDimensions() );
    //     for(UINT j=0; j<startPos.size(); j++){
    //         startPos[j] = random.getRandomNumberUniform(-1.0,1.0);
    //     }
    //
    //     //Generate the 20 time series
    //     for(UINT x=0; x<20; x++){
    //
    //         //Clear any previous timeseries
    //         trainingSample.clear();
    //
    //         //Generate the random walk
    //         UINT randomWalkLength = random.getRandomNumberInt(90, 110);
    //         VectorDouble sample = startPos;
    //         for(UINT i=0; i<randomWalkLength; i++){
    //             for(UINT j=0; j<startPos.size(); j++){
    //                 sample[j] += random.getRandomNumberUniform(-0.1,0.1);
    //             }
    //
    //             //Add the sample to the training sample
    //             trainingSample.push_back( sample );
    //         }
    //
    //         //Add the training sample to the dataset
    //         trainingData.addSample( gestureLabel, trainingSample );
    //
    //     }
    // }

    //After recording your training data you can then save it to a file
    if( !trainingData.saveDatasetToFile( "TrainingData.txt" ) ){
        cout << "Failed to save dataset to file!\n";
        return EXIT_FAILURE;
    }

    return 0;
  }
