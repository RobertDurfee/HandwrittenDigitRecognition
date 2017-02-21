# HandwrittenDigitRecognizer
C++ class for recognizing handwritten digits from 0 to 9 provided by MNIST.

### Disclaimer
This is not production-worthy code! View this simply as a proof-of-concept.

### Initialization
```C++
HandwrittenDigitRecognizer(string neuralNetworkFilename);
HandwrittenDigitRecognizer(string neuralNetworkFilename, string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE);
HandwrittenDigitRecognizer(string neuralNetworkFilename, NeuralData trainingData, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE);

HandwrittenDigitRecognizer(vector<int> sizes, string trainingDigitsRawFilename, string trainingLabelsRawFilename,int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE);
HandwrittenDigitRecognizer(vector<int> sizes, NeuralData trainingData, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE);

HandwrittenDigitRecognizer(NeuralNetwork neuralNetwork);
HandwrittenDigitRecognizer(NeuralNetwork neuralNetwork, string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE);
HandwrittenDigitRecognizer(NeuralNetwork neuralNetwork, NeuralData trainingData, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE);
```
In order to accomodate maximum flexibility, there are many ways to construct a `HandwrittenDigitRecognizer`. 

Breaking it down, there are three different tasks the constructors must accomplish:

 1. Initialize a `NeuralNetwork`
 2. Acquire training data.
 3. Set training parameters.
 
The first task is accomplished three different ways:

 1. Through a filename of a saved `NeuralNetwork`.
 2. Through a vector containing the layer dimensions.
 3. Throuh a pointer to a defined `NeuralNetwork`.

The second task is accomplished two different ways:

 1. Through two filenames of MNIST images and labels.
 2. Through a `NeuralData` of training images.
 
The last task is accomplished by either settting the `epochs`, `miniBatchSize`, and `learningRate` manually, or using recommended constants.

Of these three tasks, only the first is essential as training could have already taken place elsewhere and the loaded `NeuralNetwork` is already fully trained.

### Train
```C++
void Train(string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE);
void Train(NeuralData trainingData, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE);
```
If there is more training that needs to be accomplished or when the `HandwrittenDigitRecognizer` was constructed training had not been performed yet, these methods are available. These methods work similarly to the constructors as they must be supplied training data whether as a filename of MNIST data files or a `NeuralData` type. Training parameters can then be defined or the recommended values will be used. 

### Classify
```C++
int Classify(HandwrittenDigit digit);
```
Once the `HandwrittenDigitRecognizer` is fully constructed and trained, it can classify a `HandwrittenDigit`. This digit must be fully constructed and contain a valid 28 x 28 pixel greyscale iamge. Instructions on how to initialize a `HandwrittenDigit` can be found [here](https://github.com/RobertDurfee/HandwrittenDigit).

### Setters/Getters
```C++
void SetNeuralNetwork(string neuralNetworkFilename);
void SetNeuralNetwork(vector<int> sizes);
void SetNeuralNetwork(NeuralNetwork neuralNetwork);
NeuralNetwork * GetNeuralNetwork();
```
To redefine the `NeuralNetwork` inside the `HandwrittenDigitRecognizer` after construction, these methods are provided. After these methods are called, all previous information will be erased in the `NeuralNetwork`. If more augmenting is required, the pointer to the `NeuralNetwork` in the `HandwrittenDigitRecognizer` can be retreived. This feature should be used with caution, however.

### Deinitialization
```C++
~HandwrittenDigitRecognizer();
```
Since there are no default constructors for a `NeuralNetwork`, a pointer must be used along with the `new` operator. As a result, the `delete` operator must be called. This frees up any and all memory allocated.

### Example
```C++
#include "HandwrittenDigitRecognizer.h"
#include <iostream>

int main()
{
	HandwrittenDigitRecognizer recognizer("HandwrittenDigits.net");

	HandwrittenDigit digit((char *)"Image.bmp");
	
	cout << recognizer.Classify(digit);

	return 0;
}
```
This example uses a pre-trained `NeuralNetwork` saved to a file which, for convenience, is provided. A `HandwrittenDigit` is defined by loading a bitmap of the number seven (also provided). The result of the `Classify` method is then outputted to the standard output stream.
