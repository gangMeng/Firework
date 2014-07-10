#pragma once
#include "afxcmn.h"

#ifndef _INTERFACEMANAGER_
#include"../Interface/InterfaceManager.h"
#endif

// Config dialog

class Config : public CDialog
{
	DECLARE_DYNAMIC(Config)

public:
	Config(CWnd* pParent = NULL);   // standard constructor
	virtual ~Config();

// Dialog Data
	enum {IDD = DLG_SCRNSAVECONFIGURE};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	InterfaceManager * m_Interface;
	
	int m_MaxRockets;
	int m_StarDensity;
	int m_Smoke;
	int m_Flare;
	int m_ExplosionSmoke;
	int m_MoonGlow;
	int m_Wind;
	int m_Sound;
	int m_Ambient;
	int m_FrameRate;
	BOOL m_Moon;
	BOOL m_Cloud;
	BOOL m_Earth;
	BOOL m_Illumination;
	afx_msg void OnNMCustomdrawMaxrockets(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawStardensity(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSmokelife(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawFlare(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawExplosionsmoke1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawMoonglow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawWind(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAmbient(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawFrameratelimit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMoon();
	afx_msg void OnBnClickedClouds();
	afx_msg void OnBnClickedEarth();
	afx_msg void OnBnClickedIllumination();
	int m_MaxRocketText;
	int m_StarDensityText;
	int m_SmokeText;
	int m_FlareText;
	int m_ExplosionSmokeText;
	int m_MoonGlowText;
	int m_WindText;
	int m_SoundText;
	int m_AmbientText;
	int m_FrameRateText;
};
