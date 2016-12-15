#include <GRT/GRT.h>
using namespace std;
using namespace GRT;

int main(int argc, const char * argv[])
{
  if (argc < 3) {
    cout << "Not enough arguments! Only got " << argc-1 << " expected 2" << endl;
    return EXIT_FAILURE;
  }

  //Load a labeled data set from a txt file and train a classification model
  LabelledTimeSeriesClassificationData trainingData;
  trainingData.load(argv[1]);

  // //Setup a custom recognition pipeline
  GestureRecognitionPipeline pipeline;

  //Add a low pass filter to the pipeline with a dynamic buffer size
  pipeline << MovingAverageFilter(2, trainingData.getNumDimensions());
  //pipeline << FFT(512, 1, 3);
  pipeline << deadReckon();

  //Add a custom feature extraction algorithm that will use the output of the FFT as input
  // pipeline << MyCustomFeatureAlgorithm();

  //Add a Dynamic Time Warp Classifier and enable trimming
  DTW dtw;
  dtw.enableTrimTrainingData(true, 0.01, 90);
  //dtw.setOffsetTimeseriesUsingFirstSample(true);
  pipeline << dtw;

  // Use X% of the training dataset to create a test dataset
  LabelledTimeSeriesClassificationData testData0 = trainingData.split(75);
  //LabelledTimeSeriesClassificationData testData1 = trainingData.split(25);
  //LabelledTimeSeriesClassificationData testData2 = trainingData.split(25);
  //LabelledTimeSeriesClassificationData testData4 = trainingData.split(25);

  bool success = pipeline.train(trainingData);
  //success = pipeline.train(testData1);
  //success = pipeline.train(testData2);
  //success = pipeline.train(testData1);
  if (!success) {
    cout << "Could not train the model." << endl;
    return EXIT_FAILURE;
  }

  //Save the pipeline model to a file
  success = pipeline.save(argv[2]);
  if(!success){
      cout << "Failed to save the classifier model!\n";
      return EXIT_FAILURE;
  }

  //Load the pipline model from a file
  success = pipeline.load(argv[2]);
  if(!success){
      cout << "Failed to load the classifier model!\n";
      return EXIT_FAILURE;
  }

  // cout << "Test Accuracy: " << pipeline.getTestAccuracy() << endl;

  //Use the test dataset to test the pipeline model
  double accuracy = 0;
  for(UINT i=0; i<testData0.getNumSamples(); i++){
      //Get the i'th test sample - this is a timeseries
      UINT classLabel = testData0[i].getClassLabel();
      MatrixDouble timeseries = testData0[i].getData();

      //Perform a prediction using the classifier
      if( !pipeline.predict( timeseries ) ){
          cout << "Failed to perform prediction for test sample: " << i <<"\n";
          return EXIT_FAILURE;
      }

      //Get the predicted class label
      UINT predictedClassLabel = pipeline.getPredictedClassLabel();
      double maximumLikelihood = pipeline.getMaximumLikelihood();
      VectorDouble classLikelihoods = pipeline.getClassLikelihoods();
      VectorDouble classDistances = pipeline.getClassDistances();

      //Update the accuracy
      if( classLabel == predictedClassLabel ) accuracy++;

      cout << "TestSample: " << i <<  "\tClassLabel: " << classLabel << "\tPredictedClassLabel: " << predictedClassLabel << "\tMaximumLikelihood: " << maximumLikelihood << endl;
  }

  cout << "Test Accuracy: " << accuracy/double(testData0.getNumSamples())*100.0 << "%" << endl;


  // --------- UNCOMMENT FOR REAL TIME CLASSIFICATION ------------
  // //The following lines would be called each time the user gets a new sample
  // MatrixFloat sample = getDataFromSenor(); //Custom user function
  //
  // //Pass the sensor data down the pipeline
  // pipeline.predict( sample );
  //
  // //Get the predicted class label and likelihood
  // unsigned int predictedClassLabel = pipeline.getPredictedClassLabel();
  // Float maxLikelihood = pipeline.getMaximumLikelihood();

  return 0;
}
