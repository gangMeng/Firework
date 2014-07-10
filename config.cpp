// config.cpp : implementation file
//

#include "stdafx.h"
#include "Firework.h"
#include "config.h"

// Config dialog

IMPLEMENT_DYNAMIC(Config, CDialog)

Config::Config(CWnd* pParent /*=NULL*/)
	: CDialog(Config::IDD, pParent)
	, m_MaxRockets(8)
	, m_StarDensity(20)
	, m_Smoke(10)
	, m_Flare(20)
	, m_ExplosionSmoke(0)
	, m_MoonGlow(15)
	, m_Wind(20)
	, m_Sound(100)
	, m_Ambient(5)
	, m_FrameRate(0)
	, m_Moon(TRUE)
	, m_Cloud(TRUE)
	, m_Earth(TRUE)
	, m_Illumination(TRUE)
	, m_MaxRocketText(0)
	, m_StarDensityText(0)
	, m_SmokeText(0)
	, m_FlareText(0)
	, m_ExplosionSmokeText(0)
	, m_MoonGlowText(0)
	, m_WindText(0)
	, m_SoundText(0)
	, m_AmbientText(0)
	, m_FrameRateText(0)
{

	
	m_Interface = & theApp.m_Interface;
	
}

Config::~Config()
{
}

void Config::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Slider(pDX, MAXROCKETS, m_MaxRockets);
	DDV_MinMaxInt(pDX, m_MaxRockets, 0, 100);
	DDX_Slider(pDX, STARDENSITY, m_StarDensity);
	DDV_MinMaxInt(pDX, m_StarDensity, 0, 100);
	DDX_Slider(pDX, SMOKELIFE, m_Smoke);
	DDV_MinMaxInt(pDX, m_Smoke, 0, 100);
	DDX_Slider(pDX, FLARE, m_Flare);
	DDV_MinMaxInt(pDX, m_Flare, 0, 100);
	DDX_Slider(pDX, EXPLOSIONSMOKE1, m_ExplosionSmoke);
	DDV_MinMaxInt(pDX, m_ExplosionSmoke, 0, 100);
	DDX_Slider(pDX, MOONGLOW, m_Moon);
	DDV_MinMaxInt(pDX, m_MoonGlow, 1, 100);
	DDX_Slider(pDX, WIND, m_Wind);
	DDV_MinMaxInt(pDX, m_Wind, 0, 100);
	DDX_Slider(pDX, SOUND, m_Sound);
	DDV_MinMaxInt(pDX, m_Sound, 0, 100);
	DDX_Slider(pDX, AMBIENT, m_Ambient);
	DDV_MinMaxInt(pDX, m_Ambient, 0, 100);
	DDX_Slider(pDX, FRAMERATELIMIT, m_FrameRate);
	DDV_MinMaxInt(pDX, m_FrameRate, 0, 120);
	DDX_Check(pDX, MOON, m_Moon);
	DDX_Check(pDX, CLOUDS, m_Cloud);
	DDX_Check(pDX, EARTH, m_Earth);
	DDX_Check(pDX, ILLUMINATION, m_Illumination);
	DDX_Text(pDX, MAXROCKETSTEXT, m_MaxRocketText);
	DDX_Text(pDX, STARDENSITYTEXT, m_StarDensityText);
	DDX_Text(pDX, SMOKELIFETEXT, m_SmokeText);
	DDX_Text(pDX, FLARETEXT, m_FlareText);
	DDX_Text(pDX, EXPLOSIONSMOKETEXT, m_ExplosionSmokeText);
	DDX_Text(pDX, MOONGLOWTEXT, m_MoonGlowText);
	DDX_Text(pDX, WINDTEXT, m_WindText);
	DDX_Text(pDX, SOUNDTEXT, m_SoundText);
	DDX_Text(pDX, AMBIENTTEXT, m_AmbientText);
	DDX_Text(pDX, FRAMERATELIMITTEXT, m_FrameRateText);
}

BEGIN_MESSAGE_MAP(Config, CDialog)
	
	ON_NOTIFY(NM_CUSTOMDRAW, MAXROCKETS, &Config::OnNMCustomdrawMaxrockets)
	ON_NOTIFY(NM_CUSTOMDRAW, STARDENSITY, &Config::OnNMCustomdrawStardensity)
	ON_NOTIFY(NM_CUSTOMDRAW, SMOKELIFE, &Config::OnNMCustomdrawSmokelife)
	ON_NOTIFY(NM_CUSTOMDRAW, FLARE, &Config::OnNMCustomdrawFlare)
	ON_NOTIFY(NM_CUSTOMDRAW, EXPLOSIONSMOKE1, &Config::OnNMCustomdrawExplosionsmoke1)
	ON_NOTIFY(NM_CUSTOMDRAW, MOONGLOW, &Config::OnNMCustomdrawMoonglow)
	ON_NOTIFY(NM_CUSTOMDRAW, WIND, &Config::OnNMCustomdrawWind)
	ON_NOTIFY(NM_CUSTOMDRAW, SOUND, &Config::OnNMCustomdrawSound)
	ON_NOTIFY(NM_CUSTOMDRAW, AMBIENT, &Config::OnNMCustomdrawAmbient)
	ON_NOTIFY(NM_CUSTOMDRAW, FRAMERATELIMIT, &Config::OnNMCustomdrawFrameratelimit)
	ON_BN_CLICKED(MOON, &Config::OnBnClickedMoon)
	ON_BN_CLICKED(CLOUDS, &Config::OnBnClickedClouds)
	ON_BN_CLICKED(EARTH, &Config::OnBnClickedEarth)
	ON_BN_CLICKED(ILLUMINATION, &Config::OnBnClickedIllumination)
END_MESSAGE_MAP()




void Config::OnNMCustomdrawMaxrockets(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	UpdateData(true);//获取输入数据

    m_Interface->GetSceneManager()->SetMaxRockets(m_MaxRockets);
    m_MaxRocketText = m_MaxRockets;//更新文字显示

	UpdateData(false);//更新控件显示
	*pResult = 0;
}

void Config::OnNMCustomdrawStardensity(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	m_Interface->GetSceneManager()->SetStarDensity(m_StarDensity);
    m_StarDensityText = m_StarDensity;//更新文字显示
	*pResult = 0;
}

void Config::OnNMCustomdrawSmokelife(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(true);//获取输入数据

	m_Interface->GetSceneManager()->SetSmoke(m_Smoke);
    m_SmokeText = m_Smoke;//更新文字显示

	UpdateData(false);//更新控件显示
	
	*pResult = 0;
}

void Config::OnNMCustomdrawFlare(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(true);//获取输入数据

	m_Interface->GetSceneManager()->SetFlare(m_Flare);
    m_FlareText = m_Flare;//更新文字显示

	UpdateData(false);//更新控件显示
	*pResult = 0;
}

void Config::OnNMCustomdrawExplosionsmoke1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	UpdateData(true);//获取输入数据
	m_Interface->GetSceneManager()->SetExplosionSmoke(m_ExplosionSmoke);
    m_ExplosionSmokeText = m_ExplosionSmoke;//更新文字显示

	UpdateData(false);//更新控件显示

	*pResult = 0;
}

void Config::OnNMCustomdrawMoonglow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	UpdateData(true);//获取输入数据

	m_Interface->GetSceneManager()->SetMoonGlow(m_MoonGlow);
    m_MoonGlowText = m_MoonGlow;//更新文字显示

	UpdateData(false);//更新控件显示
	*pResult = 0;
}

void Config::OnNMCustomdrawWind(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(true);//获取输入数据

	m_Interface->GetSceneManager()->SetWind(m_Wind);
    m_WindText = m_Wind;//更新文字显示

	UpdateData(false);//更新控件显示
	*pResult = 0;
}

void Config::OnNMCustomdrawSound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(true);//获取输入数据

	m_SoundText = m_Sound;//更新文字显示

	UpdateData(false);//更新控件显示
	*pResult = 0;
}

void Config::OnNMCustomdrawAmbient(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	UpdateData(true);//获取输入数据

	m_Interface->GetSceneManager()->SetAmbient(m_Ambient);
    m_AmbientText = m_Ambient;//更新文字显示

	UpdateData(false);//更新控件显示
	*pResult = 0;
}

void Config::OnNMCustomdrawFrameratelimit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	UpdateData(true);//获取输入数据

	m_Interface->GetSceneManager()->SetFrameRate(m_FrameRate);
    m_FrameRateText = m_FrameRate;//更新文字显示

	UpdateData(false);//更新控件显示
	*pResult = 0;
}

void Config::OnBnClickedMoon()
{
  m_Interface->GetSceneManager()->CheckMoon();
}

void Config::OnBnClickedClouds()
{
  m_Interface->GetSceneManager()->CheckClouds();
}

void Config::OnBnClickedEarth()
{
  m_Interface->GetSceneManager()->CheckEarth();
}

void Config::OnBnClickedIllumination()
{
  m_Interface->GetSceneManager()->CheckIllumination();
}
