#include "PerlinNoise.h"
#include "AEEngine.h"
#include <ctime>

PerlinNoise::PerlinNoise()
{
	m_i_ArrayWidth = static_cast<int>(AEGfxGetWinMaxX()- AEGfxGetWinMinX());
	m_i_ArrayHeight = static_cast<int>(AEGfxGetWinMaxY() - AEGfxGetWinMinY());

	m_pf_NoiseSeed2D = new float[m_i_ArrayWidth * m_i_ArrayHeight];
	m_pf_PerlinNoise2D = new float[m_i_ArrayWidth * m_i_ArrayHeight];
	for (int i = 0; i < m_i_ArrayWidth * m_i_ArrayHeight; i++) m_pf_NoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;

	m_i_OutputSize = 100;//static_cast<int>(AEGfxGetWinMaxX() - AEGfxGetWinMinX());
	m_pf_NoiseSeed1D = new float[m_i_OutputSize];
	m_pf_PerlinNoise1D = new float[m_i_OutputSize];
	for (int i = 0; i < m_i_OutputSize; i++) m_pf_NoiseSeed1D[i] = (float)rand() / (float)RAND_MAX;

}


PerlinNoise::~PerlinNoise() = default;


void PerlinNoise::PerlinNoise1D(int nCount, float *fSeed, int nOctaves, float fBias, float *fOutput)
{
	//std::cout << std::endl << std::endl << "NUM_OCTAVES: " << nOctaves << std::endl;

	// Used 1D Perlin Noise
	for (int i_ArrayCount = 0; i_ArrayCount < nCount; i_ArrayCount++)
	{
		float fNoise = 0.0f;
		float fScaleAcc = 0.0f;
		float fScale = 1.0f;

		for (int i_Octaves = 0; i_Octaves < nOctaves; i_Octaves++)
		{
			int nPitch = nCount >> i_Octaves;
			int nSample1 = (i_ArrayCount / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % nCount;

			float fBlend = (float)(i_ArrayCount - nSample1) / (float)nPitch;

			float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];

			fScaleAcc += fScale;
			fNoise += fSample * fScale;
			fScale = fScale / fBias;
		}

		// Scale to seed range
		fOutput[i_ArrayCount] = fNoise / fScaleAcc;
	}
}

void PerlinNoise::PerlinNoise2D(int nWidth, int nHeight, float *fSeed, int nOctaves, float fBias, float *fOutput)
{
	// Used 1D Perlin Noise
	for (int x = 0; x < nWidth; x++)
		for (int y = 0; y < nHeight; y++)
		{
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				int nPitch = nWidth >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
				int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];
				float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];

				fScaleAcc += fScale;
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / fBias;
			}

			// Scale to seed range
			fOutput[y * nWidth + x] = fNoise / fScaleAcc;
		}

}

void PerlinNoise::Update()
{

	//srand(clock());

	if (AEInputCheckReleased(AEVK_H)) 
		for (int i = 0; i < m_i_OutputSize; i++) m_pf_NoiseSeed1D[i] = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;

	if (AEInputCheckReleased(AEVK_J)) 
		for (int i = 0; i < m_i_ArrayWidth * m_i_ArrayHeight; i++) m_pf_NoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
	
	if (AEInputCheckReleased(AEVK_I)) {
		PerlinNoise1D(m_i_OutputSize, m_pf_NoiseSeed1D, m_i_OctaveCount, m_f_ScalingBias, m_pf_PerlinNoise1D);
		for (int i_ArraySize = 0; i_ArraySize < m_i_OutputSize; i_ArraySize++)
		{
			//std::cout << m_pf_PerlinNoise1D[i_ArraySize] << " ";
		}
	}
	
	if ( AEInputCheckReleased(AEVK_K) )
		m_i_OctaveCount++;


	if (AEInputCheckReleased(AEVK_N))
		m_f_ScalingBias += 0.2f;

	if (AEInputCheckReleased(AEVK_M))
		m_f_ScalingBias -= 0.2f;

	if (m_f_ScalingBias < 0.2f)
		m_f_ScalingBias = 0.2f;

	if (m_i_OctaveCount == 9)
		m_i_OctaveCount = 1;


}