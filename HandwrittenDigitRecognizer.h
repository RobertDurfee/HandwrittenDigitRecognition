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
	HandwrittenDigitRecognizer(string);
	HandwrittenDigitRecognizer(vector<int>, string, string);
	HandwrittenDigitRecognizer(vector<int>, int, int, double, string, string);
	HandwrittenDigitRecognizer(vector<int>, vector<vector<Col<double>>>);
	HandwrittenDigitRecognizer(vector<int>, int, int, double, vector<vector<Col<double>>>);
	HandwrittenDigitRecognizer(NeuralNetwork *);
	HandwrittenDigitRecognizer(NeuralNetwork *, int, int, double, string, string);
	HandwrittenDigitRecognizer(NeuralNetwork *, string, string);
	HandwrittenDigitRecognizer(NeuralNetwork *, int, int, double, vector<vector<Col<double>>>);
	HandwrittenDigitRecognizer(NeuralNetwork *, vector<vector<Col<double>>>);

	~HandwrittenDigitRecognizer();

	int Classify(HandwrittenDigit digit);

private:
	NeuralNetwork * neuralNetwork;

	vector<vector<Col<double>>> Vectorize(char * digits, char * labels);
	char * ReadDigits(string);
	char * ReadLabels(string);
};

HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(string neuralNetworkFileDirectory)
{
	this->neuralNetwork = new NeuralNetwork(neuralNetworkFileDirectory, NULL);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory)
{
	this->neuralNetwork = new NeuralNetwork(sizes, NULL);

	char * digits = this->ReadDigits(trainingDigitsRawFileDirectory);
	char * labels = this->ReadLabels(trainingLabelsRawFileDirectory);
	
	vector<vector<Col<double>>> blank;
	this->neuralNetwork->StochasticGradientDescent(this->Vectorize(digits, labels), 100, 30, 0.1, blank);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, int epochs, int miniBatchSize, double learningRate, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory)
{
	this->neuralNetwork = new NeuralNetwork(sizes, NULL);

	char * digits = this->ReadDigits(trainingDigitsRawFileDirectory);
	char * labels = this->ReadLabels(trainingLabelsRawFileDirectory);
	
	vector<vector<Col<double>>> blank;
	this->neuralNetwork->StochasticGradientDescent(this->Vectorize(digits, labels), epochs, miniBatchSize, learningRate, blank);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, vector<vector<Col<double>>> trainingData)
{
	this->neuralNetwork = new NeuralNetwork(sizes, NULL);
		
	vector<vector<Col<double>>> blank;
	this->neuralNetwork->StochasticGradientDescent(trainingData, 100, 30, 0.1, blank);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(vector<int> sizes, int epochs, int miniBatchSize, double learningRate, vector<vector<Col<double>>> trainingData)
{
	this->neuralNetwork = new NeuralNetwork(sizes, NULL);

	vector<vector<Col<double>>> blank;
	this->neuralNetwork->StochasticGradientDescent(trainingData, epochs, miniBatchSize, learningRate, blank);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork)
{
	this->neuralNetwork = neuralNetwork;
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory)
{
	this->neuralNetwork = neuralNetwork;

	char * digits = this->ReadDigits(trainingDigitsRawFileDirectory);
	char * labels = this->ReadLabels(trainingLabelsRawFileDirectory);

	vector<vector<Col<double>>> blank;
	this->neuralNetwork->StochasticGradientDescent(this->Vectorize(digits, labels), 100, 30, 0.1, blank);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, int epochs, int miniBatchSize, double learningRate, string trainingDigitsRawFileDirectory, string trainingLabelsRawFileDirectory)
{
	this->neuralNetwork = neuralNetwork;

	char * digits = this->ReadDigits(trainingDigitsRawFileDirectory);
	char * labels = this->ReadLabels(trainingLabelsRawFileDirectory);

	vector<vector<Col<double>>> blank;
	this->neuralNetwork->StochasticGradientDescent(this->Vectorize(digits, labels), epochs, miniBatchSize, learningRate, blank);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, vector<vector<Col<double>>> trainingData)
{
	this->neuralNetwork = neuralNetwork;

	vector<vector<Col<double>>> blank;
	this->neuralNetwork->StochasticGradientDescent(trainingData, 100, 30, 0.1, blank);
}
HandwrittenDigitRecognizer::HandwrittenDigitRecognizer(NeuralNetwork * neuralNetwork, int epochs, int miniBatchSize, double learningRate, vector<vector<Col<double>>> trainingData)
{
	this->neuralNetwork = neuralNetwork;

	vector<vector<Col<double>>> blank;
	this->neuralNetwork->StochasticGradientDescent(trainingData, epochs, miniBatchSize, learningRate, blank);
}

vector<vector<Col<double>>> HandwrittenDigitRecognizer::Vectorize(char * digits, char * labels)
{
	vector<vector<Col<double>>> Output;

	for (int i = 0; i < 60000 /*Number of Images*/; i++)
	{
		Output.push_back(vector<Col<double>>());

		vector<double> temp1;

		for (int j = i * 28 /*Pixel Width*/ * 28 /*Pixel Height*/; j <= i * 28 /*Pixel Width*/ * 28 /*Pixel Height*/; j++)
			temp1.push_back((double)((unsigned char)digits[j]) / (double)256);

		Output[i].push_back(Col<double>(temp1));

		vector<double> temp2;

		for (int j = 0; j < 10 /*Possible labels*/; j++)
			temp2.push_back(0);

		temp2[labels[i]] = 1;

		Output[i].push_back(Col<double>(temp2));
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
	delete this->neuralNetwork;
}

int HandwrittenDigitRecognizer::Classify(HandwrittenDigit digit)
{
	Col<double> result = this->neuralNetwork->FeedForward(digit.GetPixelsPercentage());

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