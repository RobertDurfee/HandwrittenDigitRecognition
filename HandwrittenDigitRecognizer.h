#ifndef HANDWRITTEN_DIGIT_RECOGNIZER_HEADER
#define HANDWRITTEN_DIGIT_RECOGNIZER_HEADER

#include "ArtificialNeuralNetwork.h"  //NeuralNetwork, NeuralData, NeuralDataPoint, NeuralInput, NeuralOutput
#include "HandwrittenDigit.h"         //HandwrittenDigit
#include <string>                     //string
#include <vector>                     //vector
#include <fstream>                    //ifstream, ofstream

using namespace std;

#define RECOMMENDED_EPOCHS           100
#define RECOMMENDED_MINI_BATCH_SIZE   30
#define RECOMMENDED_LEARNING_RATE    0.1

class HandwrittenDigitRecognizer
{
public:
	HandwrittenDigitRecognizer(string neuralNetworkFilename);
	HandwrittenDigitRecognizer(string neuralNetworkFilename, string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs, int miniBatchSize, double learningRate);
	HandwrittenDigitRecognizer(string neuralNetworkFilename, NeuralData trainingData, int epochs, int miniBatchSize, double learningRate);

	HandwrittenDigitRecognizer(vector<int> sizes);
	HandwrittenDigitRecognizer(vector<int> sizes, string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs, int miniBatchSize, double learningRate);
	HandwrittenDigitRecognizer(vector<int> sizes, NeuralData trainingData, int epochs, int miniBatchSize, double learningRate);

	HandwrittenDigitRecognizer(NeuralNetwork neuralNetwork);
	HandwrittenDigitRecognizer(NeuralNetwork neuralNetwork, string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs, int miniBatchSize, double learningRate);
	HandwrittenDigitRecognizer(NeuralNetwork neuralNetwork, NeuralData trainingData, int epochs, int miniBatchSize, double learningRate);

	~HandwrittenDigitRecognizer();

	void Train(string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs, int miniBatchSize, double learningRate);
	void Train(NeuralData trainingData, int epochs, int miniBatchSize, double learningRate);

	int Classify(HandwrittenDigit digit);

	void SetNeuralNetwork(string neuralNetworkFilename);
	void SetNeuralNetwork(vector<int> sizes);
	void SetNeuralNetwork(NeuralNetwork neuralNetwork);
	NeuralNetwork * GetNeuralNetwork();

private:
	NeuralNetwork * neuralNetwork = NULL;

	NeuralData Vectorize(char * digits, char * labels);
	char * ReadDigits(string digitsRawFilename);
	char * ReadLabels(string labelsRawFilename);
};

HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(string neuralNetworkFilename)
{
	SetNeuralNetwork(neuralNetworkFilename);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(string neuralNetworkFilename, string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE)
{
	SetNeuralNetwork(neuralNetworkFilename);

	Train(trainingDigitsRawFilename, trainingLabelsRawFilename, epochs, miniBatchSize, learningRate);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(string neuralNetworkFilename, NeuralData trainingData, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE)
{
	SetNeuralNetwork(neuralNetworkFilename);

	Train(trainingData, epochs, miniBatchSize, learningRate);
}

HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> size)
{
	SetNeuralNetwork(sizes);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE)
{
	SetNeuralNetwork(sizes);

	Train(trainingDigitsRawFilename, trainingLabelsRawFilename, epochs, miniBatchSize, learningRate);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, NeuralData trainingData, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE)
{
	SetNeuralNetwork(sizes);

	Train(trainingData, epochs, miniBatchSize, learningRate);
}

HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork neuralNetwork)
{
	SetNeuralNetwork(neuralNetwork);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork neuralNetwork, string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE)
{
	SetNeuralNetwork(neuralNetwork);

	Train(trainingDigitsRawFilename, trainingLabelsRawFilename, epochs, miniBatchSize, learningRate);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork neuralNetwork, NeuralData trainingData, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE)
{
	SetNeuralNetwork(neuralNetwork);

	Train(trainingData, epochs, miniBatchSize, learningRate);
}

HandwrittenDigitRecognizer::~HandwrittenDigitRecognizer()
{
	delete neuralNetwork;
}

void HandwrittenDigitRecognizer::Train(string trainingDigitsRawFilename, string trainingLabelsRawFilename, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE)
{
	char * digits = ReadDigits(trainingDigitsRawFilename);
	char * labels = ReadLabels(trainingLabelsRawFilename);

	neuralNetwork->StochasticGradientDescent(Vectorize(digits, labels), epochs, miniBatchSize, learningRate);

	free(digits);
	free(labels);
}
void HandwrittenDigitRecognizer::Train(NeuralData trainingData, int epochs = RECOMMENDED_EPOCHS, int miniBatchSize = RECOMMENDED_MINI_BATCH_SIZE, double learningRate = RECOMMENDED_LEARNING_RATE)
{
	neuralNetwork->StochasticGradientDescent(trainingData, epochs, miniBatchSize, learningRate);
}

int HandwrittenDigitRecognizer::Classify(HandwrittenDigit digit)
{
	NeuralOutput result = neuralNetwork->FeedForward(digit.GetPixelsPercentage());

	double greatest = 0; int location = 0;
	for (int i = 0; i < 10; i++)
		if (result(i, 0) > greatest)
		{
			greatest = result(i, 0);
			location = i;
		}

	return location;
}

void HandwrittenDigitRecognizer::SetNeuralNetwork(string neuralNetworkFilename)
{
	delete neuralNetwork;
	neuralNetwork = new NeuralNetwork(neuralNetworkFilename);
}
void HandwrittenDigitRecognizer::SetNeuralNetwork(vector<int> sizes)
{
	delete neuralNetwork;
	neuralNetwork = new NeuralNetwork(sizes);
}
void HandwrittenDigitRecognizer::SetNeuralNetwork(NeuralNetwork neuralNetwork)
{
	delete this->neuralNetwork;
	this->neuralNetwork = new NeuralNetwork(neuralNetwork);
}
NeuralNetwork * HandwrittenDigitRecognizer::GetNeuralNetwork()
{
	return neuralNetwork;
}

NeuralData HandwrittenDigitRecognizer::Vectorize(char * digits, char * labels)
{
	NeuralData Output;

	for (int i = 0; i < 60000 /*Number of Images*/; i++)
	{
		Output.push_back(NeuralDataPoint());

		vector<double> temp1;

		for (int j = i * 28 /*Pixel Width*/ * 28 /*Pixel Height*/; j <= i * 28 /*Pixel Width*/ * 28 /*Pixel Height*/; j++)
			temp1.push_back((double)((unsigned char)digits[j]) / (double)256);

		Output[i].push_back(NeuralInput(temp1));

		vector<double> temp2;

		for (int j = 0; j < 10 /*Possible labels*/; j++)
			temp2.push_back(0);

		temp2[labels[i]] = 1;

		Output[i].push_back(NeuralOutput(temp2));
	}

	return Output;
}

char * HandwrittenDigitRecognizer::ReadDigits(string digitsRawFilename)
{
	ifstream digitsFile(digitsRawFilename, ios::binary);

	int header[4];	
	digitsFile.read((char *)header, sizeof(unsigned int) * 4);

	char * digits = (char *)malloc(header[1] /*Number of images*/ * 28 /*Pixel width*/ * 28 /*Pixel height*/);
	digitsFile.read(digits, header[1] /*Number of images*/ * 28 /*Pixel width*/ * 28 /*Pixel height*/);

	return digits;
}
char * HandwrittenDigitRecognizer::ReadLabels(string labelsRawFilename)
{

	ifstream labelsFile(labelsRawFilename, ios::binary);

	int header[2];	
	labelsFile.read((char *)header, sizeof(unsigned int) * 2);

	char * labels = (char *)malloc(header[1] /*Number of images*/);
	labelsFile.read(labels, header[1] /*Number of images*/);

	return labels;
}

#endif
