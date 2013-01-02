//IZQAudio.h
//�����л����Զ���ӿ�

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

// ˫����ģʽ��������
#define AM_AUDIO_DUAL_MERGE 0         //˫����
#define AM_AUDIO_DUAL_LEFT 1         //������
#define AM_AUDIO_DUAL_RIGHT 2         //������

//IZQAudioInterface �ӿڶ���
DECLARE_INTERFACE_(IZQAudioInterface,IUnknown)
{
//����������0��������������1����������2��������
STDMETHOD (put_AudioMode) (THIS_
int inAudio_Channel_Mode
) PURE;
//��������������
STDMETHOD (get_AudioMode) (THIS_
int *outAudio_Channel_Mode
) PURE;
};

#ifdef _cplusplus
}
#endif

#endif //_H_IZQAudio_

