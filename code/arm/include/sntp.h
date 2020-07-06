#ifndef SNTP_H
#define SNTP_H
/*SNTP Service
fartein.ifi.uio.no=Norway, University of Oslo 
ntp.lth.se=Sweden, Lund Institute of Technology 
time.service.uit.no=Norway, University of Tromsoe 
ntp0.pipex.net=UK, Cambridge 
ntp0.nl.net=Netherlands, Amsterdam, NLnet 
ntp2.pipex.net=UK, Cambridge 
ntp0.ja.net=UK, University of Cambridge Computer Laboratory 
ntp.cs.strath.ac.uk=Scotland, Strathclyde University, Glasgow 
ntp1.strath.ac.uk=Scotland, Glasgow, Strathclyde University 
ntp2a.mcc.ac.uk=England, University of Manchester, Manchester 
ntp4.strath.ac.uk=Scotland, Glasgow, Strathclyde University 
ntp0.strath.ac.uk=Scotland, Glasgow, Strathclyde University 
ntps1-1.cs.tu-berlin.de=Germany, Berlin, Technische Universitaet Berlin 
ntp1.pipex.net=UK, Cambridge 
ntp2.strath.ac.uk=Scotland, Glasgow, Strathclyde University 
ntp.univ-lyon1.fr=France, Lyon, CISM 
ntps1-0.cs.tu-berlin.de=Germany, Berlin, Technische Universitaet Berlin 
ntp3.strath.ac.uk=Scotland, Glasgow, Strathclyde University 
ntps1-0.uni-erlangen.de=Germany, Erlangen, University Erlangen-Nuernberg 
ntps1-2.uni-erlangen.de=Germany, Erlangen, University Erlangen-Nuernberg 
ntps1-1.uni-erlangen.de=Germany, Erlangen, University Erlangen-Nuernberg 
time.ijs.si=Slovenia, J. Stefan Institute, Ljubljana 
biofiz.mf.uni-lj.si=Slovenia, University of Ljubljana 
info.cyf-kr.edu.pl=Poland, Academic Computer Centre, Krakow 
bitsy.mit.edu=USA, MA, Cambridge, MIT Information Systems 
nic.near.net=USA, MA, Cambridge 
time-b.nist.gov= 
ntp-1.ece.cmu.edu=USA, PA, Pittsburgh, Carnegie Mellon 
swisstime.ethz.ch=Switzerland, Zurich, Integrated Systems Laboratory 
time-a.nist.gov=time-a.nist.gov 
ntp-2.ece.cmu.edu=USA, PA, Pittsburgh, Carnegie Mellon 
otc1.psu.edu=USA, PA, Penn State University 
timex.peachnet.edu=USA, GA, Kennesaw, PeachNet NOC 
fuzz.psc.edu=USA, PA, Pittsburgh, PSC Supercomputer Center 
timex.cs.columbia.edu=USA, NY, New York, Columbia University 
vtserf.cc.vt.edu=USA, VA, Virginia Tech Computing Center 
clock-1.cs.cmu.edu=USA, PA, Pittsburgh, Carnegie Mellon University 
gilbreth.ecn.purdue.edu=USA, IN, West Lafayette, Purdue University 
harbor.ecn.purdue.edu=USA, IN, West Lafayette, Purdue University 
molecule.ecn.purdue.edu=USA, IN, West Lafayette, Purdue University 
nss.unet.umn.edu=USA, MN, St. Paul 
ns.unet.umn.edu=USA, MN, Minneapolis 
clock.psu.edu=PA, Penn State University, University Park 
noc.near.net=USA, MA, Cambridge
*/
#include "types.h"
#include "user.h"
//以下定义SNTP数据结构
struct CNtpTimePacket
{
  DWORD m_dwInteger;
	DWORD m_dwFractional;
};
//Helper class to encapulate NTP time stamps
class CNtpTime
{
public:
//Constructors / Destructors
  CNtpTime();
  CNtpTime(const CNtpTime& time);
  CNtpTime(CNtpTimePacket& packet);
  CNtpTime(const SYSTEMTIME& st);

//General functions
  CNtpTime& operator=(const CNtpTime& time);
  double operator-(const CNtpTime& time) const;
  CNtpTime operator+(const double& timespan) const;
  operator SYSTEMTIME() const;
  operator CNtpTimePacket() const;
  operator unsigned int64() const { return m_Time; };
  DWORD Seconds() const;
  DWORD Fraction() const;
  void AddSpan(const double& timespan);

//Static functions
  static CNtpTime GetCurrentTime();
  static DWORD MsToNtpFraction(WORD wMilliSeconds);
  static WORD NtpFractionToMs(DWORD dwFraction);
  static double NtpFractionToSecond(DWORD dwFraction);

//The actual data
  uint64 m_Time;

protected:
//Internal static functions and data
  static long GetJulianDay(WORD Year, WORD Month, WORD Day);
  static void GetGregorianDate(long JD, WORD& Year, WORD& Month, WORD& Day);
  static DWORD m_MsToNTP[1000];

};
struct NtpServerResponse
{
  int m_nLeapIndicator; //0: no warning
                        //1: last minute in day has 61 seconds
                        //2: last minute has 59 seconds
                        //3: clock not synchronized

  int m_nStratum; //0: unspecified or unavailable
                  //1: primary reference (e.g., radio clock)
                  //2-15: secondary reference (via NTP or SNTP)
                  //16-255: reserved

  CNtpTime     m_OriginateTime;    //Time when the request was sent from the client to the SNTP server
  CNtpTime     m_ReceiveTime;      //Time when the request was received by the server
  CNtpTime     m_TransmitTime;     //Time when the server sent the request back to the client
  CNtpTime     m_DestinationTime;  //Time when the reply was received by the client
  double       m_RoundTripDelay;   //Round trip time in seconds
  double       m_LocalClockOffset; //Local clock offset relative to the server
};
//The mandatory part of an NTP packet
struct NtpBasicInfo
{
  BYTE m_LiVnMode;
  BYTE m_Stratum;
  char m_Poll;
  char m_Precision;
  long m_RootDelay;
  long m_RootDispersion;
  char m_ReferenceID[4];
  CNtpTimePacket m_ReferenceTimestamp;
  CNtpTimePacket m_OriginateTimestamp;
  CNtpTimePacket m_ReceiveTimestamp;
  CNtpTimePacket m_TransmitTimestamp;
};

//The optional part of an NTP packet
struct NtpAuthenticationInfo
{
  unsigned long m_KeyID;
  BYTE m_MessageDigest[16];
};

//The Full NTP packet
struct NtpFullPacket
{
  NtpBasicInfo          m_Basic;
  NtpAuthenticationInfo m_Auth;
};

extern char deal_sntp ( BUF_MUX *buf, uint16 port_id );
#endif
