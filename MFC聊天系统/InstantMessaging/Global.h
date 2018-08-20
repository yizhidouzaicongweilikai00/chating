#define MAXNICKNAMELENGTH				32						/// �ǳƳ���
#define HISTORYPATH						"history"				/// �����¼���ļ���
#define SENDFILES_CONFIG_SUB			"cfg"					/// �����ļ��������ļ��ĺ�׺

typedef struct tagUser											/// �û��ṹ��	
{
	int									nFace;					/// �û�ͷ��
	char								strName[ MAXNICKNAMELENGTH + 1 ];				
																/// �û���
	char								strIP[ 16 ];			/// �û�IP
	BOOL								bHasCamera;				/// �Ƿ�������ͷ
} USER;

typedef struct tabChatRoomMessageInfo								/// ��������Ϣ�ṹ��
{
	char								szSrcIP[ 16 ];			/// ��Ϣ���͵�IP
	char								szTime[ 10 ];			/// ��Ϣ���͵�ʱ��
} CHATROOMMESSAGEINFO;

enum CONTROLCOMMAND												/// ����
{
	REQUEST_ADD,												/// �����Ϊ����
	REQUEST_DENY,												/// ���󱻾ܾ�
	REQUEST_ALLOW,												/// ����ͨ��
	REQUEST_KEEPCONNECT,										/// ���󱣳ֽ���
	RESPONSE_KEEPCONNECT,										/// ��Ӧ���ֽ���
	OFFLINE,													/// ����֪ͨ
	CHATROOM_SERVER_CLOSED,										/// �����ҷ������ر�
	CHATROOM_ADDREQUEST,										/// ��������ͻ��˷�������
	CHATROOM_ADDREQUEST_DENY,									/// �ͻ��˾ܾ����������ҷ�����
	CHATROOM_ADDREQUEST_ALLOW,									/// �ͻ���������������ҷ�����
	CHATROOM_CLIENT_CLOSED,										/// �ͻ��˹ر�������
	CHATROOM_CLIENT_KICKED,										/// �ͻ��˱���
	CHATROOM_CLIENT_ADDUSER,									/// ��ͻ�������û�
	CHATROOM_CLIENT_DELDUSER,									/// �ӿͻ���ɾ���û�
	CHATROOM_TEXT,												/// �������ı���Ϣ
	CHAT_PREPARE_MESSAGE,										/// ���췢�͵���Ϣ��Ϣ
	CHAT_MESSAGE,												/// ���췢�͵���Ϣ
	CHAT_RECEIVE,												/// ��ʼ����������Ϣ������
	SENDFILES_NOTIFY,											/// ���ļ���������֪ͨ
	SENDFILES_CONNECT,											/// ���ļ������������������
	SENDFILES_DENY,												/// �ܾ������ļ�
	SENDFILES_REQUEST,											/// �����������
	SENDFILES_RESPONSE,											/// �������ݸ�������
	SENDFIELS_DONE,												/// �����������
	SENDFILES_FILEINFO,											/// ���������ͷ��ͽ��յ��ļ���Ϣ
	SENDFILES_BEGIN, 											/// ֪ͨ�����߿�ʼ���ͽ�����Ϣ
	SHARESCREEN_SERVER_CLOSED,									/// ������Ļ�������ر�
	SHARESCREEN_ADDREQUEST,										/// ������Ļ�������
	SHARESCREEN_ADDREQUEST_DENY,								/// ������Ļ�ܾ����
	SHARESCREEN_ADDREQUEST_ALLOW,								/// ������Ļ�������
	SHARESCREEN_CLIENT_CLOSED,									/// ������Ļ�ͻ��˹ر�
	SHARESCREEN_CLIENT_KICKED,									/// ������Ļ�ͻ��˱���
	SHARESCREEN_SERVER_SCREEN_DIM,								/// ������Ļ�������ĳߴ�
	SHARESCREEN_SERVER_SCREEN,									/// ������Ļ����������Ļ����
	WHITEBOARD_ADDREQUEST,										/// �װ�����û�����
	WHITEBOARD_ADDREQUEST_DENY,									/// �װ�ܾ��������
	WHITEBOARD_ADDREQUEST_ALLOW,								/// �װ������������
	WHITEBOARD_SERVER_CLOSED,									/// �װ�������ر�
	WHITEBOARD_CLIENT_COLSED,									/// �װ�ͻ��˹ر�
	WHITEBOARD_CLIENT_KICKED,									/// �װ�ͻ��˱���
	WHITEBOARD_DRAWINFO,										/// ��ͼ��Ϣ
	VIDEOCHAT_VIDEO_ADDREQUEST,									/// ��Ƶ����
	VIDEOCHAT_VIDEO_ADDREQUEST_DENY,							/// �ܾ���Ƶ����
	VIDEOCHAT_VIDEO_ADDREQUEST_ALLOW,							/// ������Ƶ����
	VIDEOCHAT_VIDEO_PICTURE_START,								/// ��Ҫ������Ƶ����
	VIDEOCHAT_VIEDO_PICTURE_REQUEST,							/// ��������Ƶ����
	VIDEOCHAT_VIDEO_PICTURE,									/// ��Ƶ��������
	VIDEOCHAT_VIDEO_DISCONNECT,									/// �Ͽ�����
	VIDEOCHAT_AUDIO_DATA,										/// ��Ƶ����
	VIDEOCHAT_VIDEO_RECEIVE_FINISHED							/// ���ݽ������
};

typedef struct tagDataPacket									/// ���ݰ��Ľṹ��
{
	CONTROLCOMMAND command;										/// ����
	BYTE * pData;												/// ���ݵ�ָ��
	DWORD dwLength;												/// ���ݵĳ���
} DATAPACKET;

typedef struct tagChatPacketInfo								/// ���촫����Ϣ�ṹ
{
	int									nCurrent;				/// ��ǰ�����
	int									nTotal;					/// ��������
	int									nLength;				/// ��ǰ���ĳ���
} CHATPACKETINFO;

typedef struct tagSendFilesList									/// �ļ����Ϳ�Ľṹ
{	
	int									nIndex;					/// ���
	char								szFilePath[ MAX_PATH ];	/// �ļ�·��
	DWORD								dwLength;				/// ��С
	DWORD								dwSended;				/// �����
	int									nPercent;				/// ����
	char								strName[ MAXNICKNAMELENGTH + 1 ];				
																/// �����û���
	char								strIP[ 16 ];			/// ����IP		
} SENDFILESLIST;

enum TOOL														/// ����
{
	ERASER,														/// ��Ƥ��
	PENCIL,														/// ����
	LINE,														/// ֱ��
	RECTANGLE_EMPTY,											/// ���ľ���
	RECTANGLE,													/// ʵ�ľ���
	ELLIPSE_EMPTY,												/// ������Բ
	ELLIPSE														/// ʵ����Բ
};

typedef struct tagDrawInfo										/// ��ͼ����Ϣ
{
	TOOL								tool;					/// ����
	int									nWidth;					/// �߿�
	COLORREF							color;					/// ��ɫ
	CPoint								ptStart;				/// ��ʼ��
	CPoint								ptEnd;					/// ������
} DRAWINFO;

#define TIMER_CONNECT					1						/// �������ӵ�timer��
#define TIMER_TRAYSHOW					2						/// ���̶�̬ͼ�����ʾtimer��
#define TIMER_SENDFILES_REFRESH			3						/// �����ļ������б��
#define TIMER_SHARESCREEN				4						/// ������Ļ��timer��
#define TIMER_VIDEOCHAT					5						/// ��Ƶ�����timer��

#define SHARESCREEN_BITCOUNT			8						/// ������Ļʱ��ɫ��λ��

#define DEFAULT_WHITEBOARD_CANVAS_COLOR	RGB( 255, 255, 255 )	/// Ĭ�ϰװ廭����ɫ
#define DEFAULT_WHITEBOARD_TOOL_COLOR	RGB( 255, 0, 0 )		/// Ĭ�ϰװ幤����ɫ
#define WHITEBOARD_WIDTH_1				1						/// �װ��߿�һ
#define WHITEBOARD_WIDTH_2				( WHITEBOARD_WIDTH_1 + 6)
																/// �װ��߿��
#define WHITEBOARD_WIDTH_3				( WHITEBOARD_WIDTH_2 + 6)
																/// �װ��߿���
#define WHITEBOARD_WIDTH_4				( WHITEBOARD_WIDTH_3 + 6)
																/// �װ��߿���
#define WHITEBOARD_MIN_WIDTH			300						/// �װ崰����С���
#define WHITEBOARD_MIN_HEIGHT			450						/// �װ崰����С�߶�

#define VIDEOCHAT_VIDEO_WIDTH			320						/// ��Ƶ�Ŀ�
#define VIDEOCHAT_VIDEO_HEIGHT			240						/// ��Ƶ�ĳ�
#define VIDEOCHAT_VIDEOWND_WIDTH		( 320 + 2 * 2 )			/// ��Ƶ���ڵĿ�
#define VIDEOCHAT_VIDEOWND_HEIGHT		( 240 + 2 * 2 )			/// ��Ƶ���ڵĳ�

#define AUDIO_BUFFER_SIZE				16384					/// ��Ƶ����������
#define AUDIO_BUFFER_BLOCK				5						/// ������Ƶ���ݵĻ�������

#define MAXCHATLENGTH					512						/// ���췢����Ϣÿ�ε���󳤶�

#define DEFAULT_REFRESH_TIME			( 1000 * 5 )			/// ˢ����ϵ���б��ʱ��,��λ����
#define DEFAULT_SHARESCREEN_REFRESH		( 200 )					/// ������Ļˢ�µ�ʱ��
#define DEFALUT_VIDEOCHAT_REFRESH		( 2000 )				/// ��Ƶ����ˢ��			

#define MAXDATAPACKETLENGTH				( 38 + 512 )			/// ���ݰ�����󳤶�

#define WM_SHELLNOTIFY					WM_USER+1				/// ������Ϣ
#define TM_SENDVIDEO_RECEIVE			WM_USER+2				/// ������Ƶ�����̵߳Ľ���������Ϣ
#define TM_SENDVIDEO_UPDATEPICTURE		WM_USER+3				/// ������Ƶ�����̵߳ĸ�����Ƶ�������Ϣ
#define TM_SENDVIDEO_DELUSER			WM_USER+4				/// ������Ƶ�����̵߳�ɾ���û�����Ϣ
#define TM_SENDVIDEO_SENDAUDIO			WM_USER+5				/// ������Ƶ�����̵߳ķ�����Ƶ���ݵ���Ϣ
#define TM_SENDVIDEO_EXIT				WM_USER+6				/// ������Ƶ�����̵߳��˳���Ϣ
#define TM_RECEIVEVIDEO_RECEIVE			WM_USER+7				/// ������Ƶ�����̵߳Ľ���������Ϣ
#define TM_RECEIVEVIDEO_DELUSER			WM_USER+8				/// ������Ƶ�����̵߳�ɾ���û�����Ϣ
#define TM_RECEIVEVIDEO_EXIT			WM_USER+9				/// ������Ƶ�����̵߳��˳���Ϣ

#define CONFIG_FILE						"conf.ini"				/// �����ļ�����

#define	DEFAULT_VIRTUALKEYCODE			'Z'						/// Ĭ���ȼ��������
#define	DEFAULT_MODIFIERS				MOD_CONTROL | \
										MOD_ALT					/// Ĭ���ȼ���������־
#define DEFAULT_AUTOADDFRIENDS			0						/// Ĭ���Զ���Ϊ����

#define STATE_ONLINE					0						/// ����״̬
#define STATE_OFFLINE					1						/// ����״̬

#define DEFAULT_FACE					0						/// Ĭ��ͷ��
#define DEFAULT_STATE					STATE_ONLINE			/// Ĭ��״̬
#define DEFAULT_NICKNAME				"����"					/// Ĭ���ǳ�
#define MAXHISTORYFRIENDS				10						/// ��ϵ����ʷ��¼���ֵ
#define MAXFACES						83						/// ���ͷ����

#define IDHOTKEY						100						/// �ȼ���ID
#define	FACEBGCOLOR						RGB( 0, 128, 128 )		/// ͷ�񱳾���ɫ

#define DEFAULT_FRIENDSLIST_HEIGHT		( 32 + 5 + 5 )			/// Ĭ���û��б�ĸ߶�

#define DEFALUT_FRIENDSLIST_NAMECOLOR	RGB( 0, 0, 0 )			/// �û��б��û�������ɫ
#define DEFALUT_FRIENDSLIST_IPCOLOR		RGB( 128, 128, 128 )	/// �û��б��û�IP����ɫ
#define DEFAULT_FRIENDSLIST_NORMALCOLOR	RGB( 255, 255, 255 )	/// �û��б������ı���ɫ
#define DEFAULT_FRIENDSLIST_SELCOLOR	RGB( 252, 234, 162 )	/// �û��б�ѡ�еı���ɫ
#define DEFAULT_FRIENDSLIST_HOVERCOLOR	RGB( 197, 227, 247 )	/// �û��б���껬������ɫ
#define DEFAULT_FRIENDSLIST_FONT		100						/// �û��б��û���IP�������С

#define DEFAULT_PORT					8000					/// Ĭ�϶˿ں�
#define CHATROOM_SERVER_PORT			( DEFAULT_PORT + 1 )	/// �����ҷ������˿ں�
#define CHATROOM_CLIENT_PORT			( DEFAULT_PORT + 2 )	/// �����ҿͻ��˶˿ں�
#define SENDFILESSERVER_PORT			( DEFAULT_PORT + 3 )	/// �����ļ��������Ķ˿ں�
#define SHARESCREENSERVER_PORT			( DEFAULT_PORT + 4 )	/// ������Ļ�������˿ں�
#define SHARESCREENCLIENT_PORT			( DEFAULT_PORT + 5 )	/// ������Ļ�ͻ��˶˿ں�
#define WHITEBOARD_SERVER_PORT			( DEFAULT_PORT + 6 )	/// �װ�������˿ں�
#define WHITEBOARD_CLIENT_PORT			( DEFAULT_PORT + 7 )	/// �װ�ͻ��˶˿ں�
#define VIDEOCHAT_PORT					( DEFAULT_PORT + 8 )	/// ��Ƶ����˿ں�
