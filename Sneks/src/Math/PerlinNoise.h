#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H
#pragma once
class PerlinNoise
{
private:
	// 2D noise variables
	int    m_i_ArrayWidth;
	int    m_i_ArrayHeight;
	float *m_pf_NoiseSeed2D   = nullptr;
	float *m_pf_PerlinNoise2D = nullptr;

	// 1D noise variables
	float *m_pf_NoiseSeed1D   = nullptr;
	float *m_pf_PerlinNoise1D = nullptr;
	int    m_i_OutputSize; // How big the array we want it to be

	int    m_i_OctaveCount    = 1;
	float  m_f_ScalingBias    = 2.0f;
	int    m_i_Mode           = 1;

public:
	PerlinNoise();
	~PerlinNoise();
	void PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput);
	void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput);
	void Update();
};

#endif //PERLIN_NOISE_H