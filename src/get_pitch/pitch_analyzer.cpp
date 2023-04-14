/// @file

#include <iostream>
#include <math.h>
#include "pitch_analyzer.h"

using namespace std;

/// Name space of UPC
namespace upc {
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) {
  		/// \TODO Compute the autocorrelation r[l]
      //1r: inicializamos a 0
      //2n: acumular valores
      //3r: dividimos por la longitud

      r[l] = 0;

      for(unsigned int n = l; n<x.size(); n++){
        r[l] += x[n] * x[n-l];
      }
      r[l] /= x.size();

    }

    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 
  }

  void PitchAnalyzer::set_window(Window win_type) {
    if (frameLen == 0)
      return;

    window.resize(frameLen);

    

    switch (win_type) {
    case HAMMING:
      /// \TODO Implement the Hamming window
      //Ventana de Hamming
      for(unsigned int i=0; i<frameLen; i++){
        window[i] = 0.54 - 0.46*cos(2*M_PI*i/(frameLen - 1)); //Función ventana de Hamming
      }
      break;
    case RECT:
    default:
      window.assign(frameLen, 1);
    }
  }

  void PitchAnalyzer::set_f0_range(float min_F0, float max_F0) {
    npitch_min = (unsigned int) samplingFreq/max_F0;
    if (npitch_min < 2)
      npitch_min = 2;  // samplingFreq/2

    npitch_max = 1 + (unsigned int) samplingFreq/min_F0;

    //frameLen should include at least 2*T0
    if (npitch_max > frameLen/2)
      npitch_max = frameLen/2;
  }

  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm) const {
    /// \TODO Implement a rule to decide whether the sound is voiced or not.
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.

    float u_maxnorm = 0.4;
    float u_r1norm = 0.6;
    
    if(rmaxnorm >= u_maxnorm || r1norm >= u_r1norm)
      return false; //sonoro
    else
      return true;
  }

  float PitchAnalyzer::compute_pitch(vector<float> & x) const { 
    //Compute pitch calcula la autocorrelación
    if (x.size() != frameLen)
      return -1.0F;

    //Frame center-clipping
    float max = *std::max_element(x.begin(), x.end());
    for(int i = 0; i < (int)x.size(); i++) {
      if(abs(x[i]) < cclip) {
        x[i] = 0.0F;
      }
    }

    //Frame normalization
    max = *std::max_element(x.begin(), x.end());
    for (int i = 0; i < (int)x.size(); i++)
      x[i] /= max;

    //Window input frame
    for (unsigned int i=0; i<x.size(); ++i)
      x[i] *= window[i];

    vector<float> r(npitch_max);

    //Compute correlation
    autocorrelation(x, r);

    vector<float>::const_iterator iR = r.begin(), iRMax = iR + npitch_min;

    /// \TODO 
	/// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
	/// Choices to set the minimum value of the lag are:
	///    - The first negative value of the autocorrelation.
	///    - The lag corresponding to the maximum value of the pitch.
    ///	   .
	/// In either case, the lag should not exceed that of the minimum value of the pitch.

  //El máximo tiene que estar situado entre el pitch mínimo y máximo, por lo tanto,
  //begin() se usa para devolver un iterador que apunta al primer elemento del vector
  for(iR = iRMax = (r.begin() + npitch_min); iR < (r.begin() + npitch_max); iR++){
    if (*iR > *iRMax)
      iRMax = iR;
  }

    unsigned int lag = iRMax - r.begin(); //Diferencia entre la posición del valor más alto y la posición inicial

    float pot = 10 * log10(r[0]);

    //You can print these (and other) features, look at them using wavesurfer
    //Based on that, implement a rule for unvoiced
    //change to #if 1 and compile
#if 0
    if (r[0] > 0.0F)
      cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;
#endif
    
    if (unvoiced(pot, r[1]/r[0], r[lag]/r[0]))
      return 0;
    else
      return (float) samplingFreq/(float) lag;
  }
}
