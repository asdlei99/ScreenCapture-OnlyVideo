#pragma once


#include <Windows.h>
#include <atomic>
#include <QObject>
#include <QString>
#include <QMutex>

#ifdef	__cplusplus
extern "C"
{
#endif
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVFifoBuffer;
struct AVAudioFifo;
struct AVFrame;
struct SwsContext;
struct SwrContext;
#ifdef __cplusplus
};
#endif

class ScreenRecordImpl : public QObject
{
	Q_OBJECT
public:
	ScreenRecordImpl(QObject * parent = Q_NULLPTR);

	int OpenVideo();
	int OpenOutput();

	private slots :
	void Start();
	void Finish();

private:
	//��fifobuf��ȡ��Ƶ֡������д��������������ļ�
	void EncodeThreadProc();
	//����Ƶ��������ȡ֡��д��fifobuf
	void ScreenRecordThreadProc();
	void FlushDecoder();
	void FlushEncoder();

private:
	QString				m_filePath;
	int					m_width;
	int					m_height;
	int					m_fps;

	int m_vIndex;		//������Ƶ������
	int m_vOutIndex;	//�����Ƶ������
	AVFormatContext		*m_vFmtCtx;
	AVFormatContext		*m_oFmtCtx;
	AVCodecContext		*m_vDecodeCtx;
	AVCodecContext		*m_vEncodeCtx;
	SwsContext			*m_swsCtx;
	AVFifoBuffer		*m_vFifoBuf;

	AVFrame				*m_vOutFrame;
	uint8_t				*m_vOutFrameBuf;
	int					m_vOutFrameSize;
	std::atomic_bool	m_stop;
	CRITICAL_SECTION	m_vSection;
	int					m_vFrameIndex;	//��ǰ֡λ��

	bool				m_started;
};