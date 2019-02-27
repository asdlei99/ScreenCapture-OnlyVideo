#pragma once


#include <Windows.h>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <QObject>
#include <QString>
#include <QMutex>
#include <QVariant>

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
struct AVDictionary;
#ifdef __cplusplus
};
#endif

class ScreenRecordImpl : public QObject
{
	Q_OBJECT
public:
	enum RecordState {
		NotStarted,
		Started,
		Paused,
		Stopped,
		Unknown,
	};

	ScreenRecordImpl(QObject * parent = Q_NULLPTR);
	//��ʼ����Ƶ����
	void Init(const QVariantMap& map);

	private slots :
	void Start();
	void Pause();
	void Stop();

private:
	//��fifobuf��ȡ��Ƶ֡������д��������������ļ�
	void ScreenRecordThreadProc();
	//����Ƶ��������ȡ֡��д��fifobuf
	void ScreenAcquireThreadProc();
	int OpenVideo();
	int OpenOutput();
	void SetEncoderParm();
	void FlushDecoder();
	void FlushEncoder();
	void InitBuffer();
	void Release();

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
	AVDictionary		*m_dict;
	SwsContext			*m_swsCtx;
	AVFifoBuffer		*m_vFifoBuf;
	AVFrame				*m_vOutFrame;
	uint8_t				*m_vOutFrameBuf;
	int					m_vOutFrameSize;	//һ�����֡���ֽ�
	RecordState			m_state;

	//�����ٶ�һ��Ȳɼ��ٶ��������Կ���ȥ��m_cvNotEmpty
	std::condition_variable m_cvNotFull;
	std::condition_variable m_cvNotEmpty;
	std::mutex				m_mtx;
	std::condition_variable m_cvNotPause;
	std::mutex				m_mtxPause;
};