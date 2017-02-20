#ifndef HANDWRITTEN_DIGIT_RECOGNIZER_HEADER
#define HANDWRITTEN_DIGIT_RECOGNIZER_HEADER

#include "ArtificialNeuralNetwork.h"
#include "HandwrittenDigit.h"
#include <armadillo>
#include <string>
#include <vector>
#include <fstream>

using namespace arma;
using namespace std;

class HandwrittenDigitRecognizer
{
public:
	HandwrittenDigitRecognizer(string neuralNetworkFileDirectory);
	HandwrittenDigitRecognizer(vector<int> sizes, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory);
	HandwrittenDigitRecognizer(vector<int> sizes, int epochs, int miniBatchSize, double learningRate, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory);
	HandwrittenDigitRecognizer(vector<int> sizes, NeuralData trainingData);
	HandwrittenDigitRecognizer(vector<int> sizes, int epochs, int miniBatchSize, double learningRate, NeuralData trainingData);
	HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork);
	HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, int sizes, int miniBatchSize, double learningRate, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory);
	HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory);
	HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, int epochs, int miniBatchSize, double learningRate, NeuralData trainingData);
	HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, NeuralData trainingData);

	~HandwrittenDigitRecognizer();

	int Classify(HandwrittenDigit digit);

private:
	NeuralNetwork * neuralNetwork;

	NeuralData Vectorize(char * digits, char * labels);
	char * ReadDigits(string digitsRawFileDirectory);
	char * ReadLabels(string labelsRawFileDirectory);
};

HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(string neuralNetworkFileDirectory)
{
	neuralNetwork = new NeuralNetwork(neuralNetworkFileDirectory, NULL);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory)
{
	neuralNetwork = new NeuralNetwork(sizes, NULL);

	char * digits = ReadDigits(trainingDigitsRawFileDirectory);
	char * labels = ReadLabels(trainingLabelsRawFileDirectory);
	
	neuralNetwork->StochasticGradientDescent(Vectorize(digits, labels), 100, 30, 0.1);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, int epochs, int miniBatchSize, double learningRate, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory)
{
	neuralNetwork = new NeuralNetwork(sizes, NULL);

	char * digits = ReadDigits(trainingDigitsRawFileDirectory);
	char * labels = ReadLabels(trainingLabelsRawFileDirectory);
	
	neuralNetwork->StochasticGradientDescent(Vectorize(digits, labels), epochs, miniBatchSize, learningRate);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, NeuralData trainingData)
{
	neuralNetwork = new NeuralNetwork(sizes, NULL);
		
	neuralNetwork->StochasticGradientDescent(trainingData, 100, 30, 0.1);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, int epochs, int miniBatchSize, double learningRate, NeuralData trainingData)
{
	neuralNetwork = new NeuralNetwork(sizes, NULL);

	neuralNetwork->StochasticGradientDescent(trainingData, epochs, miniBatchSize, learningRate);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork)
{
	this->neuralNetwork = neuralNetwork;
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory)
{
	this->neuralNetwork = neuralNetwork;

	char * digits = ReadDigits(trainingDigitsRawFileDirectory);
	char * labels = ReadLabels(trainingLabelsRawFileDirectory);

	this->neuralNetwork->StochasticGradientDescent(Vectorize(digits, labels), 100, 30, 0.1);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, int epochs, int miniBatchSize, double learningRate, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory)
{
	this->neuralNetwork = neuralNetwork;

	char * digits = ReadDigits(trainingDigitsRawFileDirectory);
	char * labels = ReadLabels(trainingLabelsRawFileDirectory);

	this->neuralNetwork->StochasticGradientDescent(Vectorize(digits, labels), epochs, miniBatchSize, learningRate);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, NeuralData trainingData)
{
	this->neuralNetwork = neuralNetwork;

	vector<vector<Col<double>>> blank;
	this->neuralNetwork->StochasticGradientDescent(trainingData, 100, 30, 0.1, blank);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, int epochs, int miniBatchSize, double learningRate, NeuralData trainingData)
{
	this->neuralNetwork = neuralNetwork;

	this->neuralNetwork->StochasticGradientDescent(trainingData, epochs, miniBatchSize, learningRate);
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
char * HandwrittenDigitRecognizer::ReadDigits(string digitsRawFileDirectory)
{
	ifstream digitsFile(digitsRawFileDirectory, ios::binary);

	int header[4];
	char * digits = (char *)malloc(60000 /*Number of images*/ * 28 /*Pixel width*/ * 28 /*Pixel height*/);
	
	digitsFile.read((char *)header, sizeof(unsigned int) * 4);
	digitsFile.read(digits, 60000 /*Number of images*/ * 28 /*Pixel width*/ * 28 /*Pixel height*/);

	return digits;
}
char * HandwrittenDigitRecognizer::ReadLabels(string labelsRawFileDirectory)
{

	ifstream labelsFile(labelsRawFileDirectory, ios::binary);

	int header[2];
	char * labels = (char *)malloc(60000 /*Number of images*/);
	
	labelsFile.read((char *)header, sizeof(unsigned int) * 2);
	labelsFile.read(labels, 60000 /*Number of images*/);

	return labels;
}

HandwrittenDigitRecognizer::~HandwrittenDigitRecognizer()
{
	delete neuralNetwork;
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

#endif