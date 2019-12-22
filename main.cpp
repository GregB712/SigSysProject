#include <iostream>
#include <vector>
#include <random>
#include "AudioFile.h"
#include <chrono>

using namespace std::chrono;

using namespace std;

template<typename T>
vector<T> MyConvolution(vector<T> &u, vector<T> &v);

void Convolution_of_A_and_B();
void Convolution_of_Sample_and_Pink();
void Convolution_of_Sample_and_White();

default_random_engine generator;
uniform_real_distribution<double> distribution(-1,1); //doubles from -1 to 1

int main() {
    cout<< "Signals and Systems Project" <<endl;
    Convolution_of_A_and_B();
    Convolution_of_Sample_and_Pink();
    Convolution_of_Sample_and_White();
    return 0;
}

/*
 * The first task of the project.
 */

void Convolution_of_A_and_B(){
    int size_A;
    do {
        cout << "Give size of vector A: ";
        cin >> size_A; //Get size of vector A from user
        if(size_A < 10)
            cout<<"Size of vector must be bigger or equal of 10"<<endl;
    } while (size_A < 10);


    //Create the vector A
    vector <float> A;
    A.reserve(size_A);
    for (int i=0; i < size_A; i++){
        A.push_back((float)rand() / (RAND_MAX));
    }

    //Create the vector B
    vector <float> B;
    int size_B = 5;
    B.reserve(size_B);
    for(int i=0; i < size_B; i++){
        B.push_back(0.2);
    }

    //Print the results of the convolution
    vector<float> C;
    auto start = high_resolution_clock::now();

    C = MyConvolution(A,B);

    // Get ending timepoint
    auto stop = high_resolution_clock::now();

    // Get duration. Substart timepoints to
    // get durarion. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Convolution_of_A_and_B: " << duration.count() << " nanoseconds" << endl;
    /*for(float q : C){
        cout<<q<<endl;
    }*/
}

/*
 * The second task of the project.
 */
void Convolution_of_Sample_and_Pink(){
    //Loading the raw data from wav files to vectors.
    int channel = 0;
    AudioFile<float> sample_audio;
    sample_audio.load("./sample_audio.wav");


    AudioFile<float> pink_noise;
    pink_noise.load("./pink_noise.wav");

    //Convolution of sample_audio and pink_noise.
    AudioFile<float> pinkNoise_sampleAudio;
    auto start = high_resolution_clock::now();
    pinkNoise_sampleAudio.samples[channel] = MyConvolution(sample_audio.samples[channel], pink_noise.samples[channel]);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Convolution_of_Sample_and_Pink: " << duration.count() << " milliseconds" << endl;
    //Saving the new wav file.
    pinkNoise_sampleAudio.save("pinkNoise_sampleAudio.wav");
}

void Convolution_of_Sample_and_White(){
    //Loading the raw data from wav file to vector.
    int channel = 0;
    AudioFile<float> sample_audio;
    sample_audio.load("./sample_audio.wav");

    //Generate white noise sample.
    double random_number;
    int size_vector_white_noise = 1000;
    vector<float> vector_white_noise;
    vector_white_noise.reserve(size_vector_white_noise);
    for (int i=0; i < size_vector_white_noise; i++){
        random_number = distribution(generator);
        vector_white_noise.push_back(random_number);
    }

    //Convolution of sample_audio and white_noise.
    AudioFile<float> whiteNoise_sampleAudio;
    auto start = high_resolution_clock::now();
    whiteNoise_sampleAudio.samples[channel] = MyConvolution(sample_audio.samples[channel], vector_white_noise);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Convolution_of_Sample_and_White: " << duration.count() << " milliseconds" << endl;
    //Saving the new wav file.
    whiteNoise_sampleAudio.save("whiteNoise_sampleAudio.wav");
}

/*
 * Function with two vectors as arguments that returns the convolution of them.
 */
template<typename T>
vector<T> MyConvolution(vector<T> &u, vector<T> &v){
    int const u_size = u.size();
    int const v_size = v.size();
    int const final_size = u_size + v_size - 1;
    int moving;
    vector <T> w(final_size);
    w.reserve(final_size);
    for (auto i(0); i<final_size; i++){
        moving = i;
        for (auto j(0); j<v_size; j++){
            if(moving>=0 && moving<u_size)
                w[i] = w[i] + (u[moving]*v[j]);
            moving = moving-1;
        }
    }
    return w;
}

