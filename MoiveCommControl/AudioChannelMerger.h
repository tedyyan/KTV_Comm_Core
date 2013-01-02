//IZQAudio.h
//声道切换的自定义接口

#ifndef _H_IZQAudioInterface_
#define _H_IZQAudioInterface_

#ifdef _cplusplus
extern "C" {
#endif
// {CFBE95E1-5DB4-11d8-929C-92B98A07327D}
DEFINE_GUID(CLSID_ZQAudio, 
0xcfbe95e1, 0x5db4, 0x11d8, 0x92, 0x9c, 0x92, 0xb9, 0x8a, 0x7, 0x32, 0x7d);
//IZQAudioChannel's GUID
// {83BA1141-6135-11d8-929C-A23780A5EB7C}
DEFINE_GUID(IID_IZQAudioInterface, 
0x83ba1141, 0x6135, 0x11d8, 0x92, 0x9c, 0xa2, 0x37, 0x80, 0xa5, 0xeb, 0x7c);

// 双声道模式参数声明
#define AM_AUDIO_DUAL_MERGE 0         //双声道
#define AM_AUDIO_DUAL_LEFT 1         //左声道
#define AM_AUDIO_DUAL_RIGHT 2         //右声道

//IZQAudioInterface 接口定义
DECLARE_INTERFACE_(IZQAudioInterface,IUnknown)
{
//设置声道：0－不处理声道，1－左声道，2－右声道
STDMETHOD (put_AudioMode) (THIS_
int inAudio_Channel_Mode
) PURE;
//返回声道的设置
STDMETHOD (get_AudioMode) (THIS_
int *outAudio_Channel_Mode
) PURE;
};

#ifdef _cplusplus
}
#endif

#endif //_H_IZQAudio_

