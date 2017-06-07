#ifndef GOBJECTCOMMON_H_HEADER_INCLUDED_BC939898
#define GOBJECTCOMMON_H_HEADER_INCLUDED_BC939898

//##ModelId=4340AAE401C5
//##Documentation
//## ���壺
//##       ͼԪ���Ͷ���
//## ���ܣ�
//##       ����ͼԪ����
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-16
enum  { 
    //##Documentation
    //## δ֪ͼԪ����
    GD_TYPE_UNKNOWN = -1, 
    //##Documentation
    //## ����ͼԪ��
    GD_TYPE_LINE, 
    //##Documentation
    //## ����ͼԪ��Բ
    GD_TYPE_ELLIPSE, 
    //##Documentation
    //## ����ͼԪԲ
    GD_TYPE_CIRCLE, 
    //##Documentation
    //## ����ͼԪ����
    GD_TYPE_RECT, 
    //##Documentation
    //## Բ�Ǿ���
    GD_TYPE_ROUNDRECT, 
    //##Documentation
    //## ����ͼԪ����
    GD_TYPE_POLYLINE, 
    //##Documentation
    //## ����ͼԪ�����
    GD_TYPE_POLYGON, 
    //##Documentation
    //## ����ͼԪ����
    GD_TYPE_ARC, 
    //##Documentation
    //## ����ͼԪ��ť
    GD_TYPE_BUTTON, 
    //##Documentation
    //## ����ͼԪ��̬�ı�
    GD_TYPE_TEXT, 
    //##Documentation
    //## ����ͼԪͼ��
    GD_TYPE_IMAGE, 
    //##Documentation
    //## ����ͼԪ��̬�ı�
    GD_TYPE_DYNTEXT, 
    //##Documentation
    //## ճ���
    GD_TYPE_PIN,
	//##Documentation
    //## ĸ��
	GD_TYPE_BUS,
	//##Documentation
    //## ������
	GD_TYPE_LINK,
	//##Documentation
    //## ������
	GD_TYPE_FLOW,
    //##Documentation
    //## ���ͼԪ
    GD_TYPE_COMOBJ, 
    //##Documentation
    //## �Զ���ͼԪ
    GD_TYPE_CUSTOM,
	//##Documentation
    //## ���ͼԪ
    GD_TYPE_PLUGIN,
	//##Documentation
    //## ��������
	GD_TYPE_BEZIER,
	//##Documentation
	//##Բ��ͼԪ
	GD_TYPE_CARC,
	//##Documentation
	//##·��ͼԪ
	GD_TYPE_PATH,
	//���ͼԪ
	GD_TYPE_BAY};

//##ModelId=4340AAE401C5
//##Documentation
//## ���壺
//##       �豸���Ͷ���
//## ���ܣ�
//##       �����豸����
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-16
enum{
	//##Documentation
    //## δ֪�豸����
	GD_DEVTYPE_UNKNOWN=0};

//##ModelId=4340AAE401C5
//##Documentation
//## ���壺
//##       �豸״̬����
//## ���ܣ�
//##       �����豸״̬
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-16
enum{
	//##Documentation
    //## δ֪�豸״̬
	GD_DEVSTATUS_UNKNOWN=0x00,
	//##Documentation
    //## ����������״̬
	GD_DEVSTATUS_NOTTOPO=0x01};

//##ModelId=436C57150213
//##Documentation
//## ���壺
//##        ���ͷ����
//## ���ܣ�
//##        �������ͷ��
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-10-3
enum  { 
    //##Documentation
    //## �ջ���
    GLS_NULL , 
    //##Documentation
    //## ʵ����
    GLS_SOLID , 
    //##Documentation
    //## ���߻���
    GLS_DASH, 
    //##Documentation
    //## �㻭��
    GLS_DOT, 
    //##Documentation
    //## �۵���
    GLS_DASHDOT, 
    //##Documentation
    //## �۵����
    GLS_DASHDOTDOT,
	//##Documentation
    //## ���߿�����
	GLS_DASHDASH};

//##ModelId=436C59200128
//##Documentation
//## ���壺
//##        ��ͷ��״����
//## ���ܣ�
//##        �����ͷ��״
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
enum  { 
    //##Documentation
    //## �ռ�ͷ
    GLA_NULL    = 0, 
    //##Documentation
    //## ��ʵ�ļ�ͷ
    GLA_PURE    = 1, 
    //##Documentation
    //## �򿪿��ļ�ͷ
    GLA_OPEN    = 2, 
    //##Documentation
    //## ����ͷ
    GLA_STEALTH = 3, 
    //##Documentation
    //## ���μ�ͷ
    GLA_DIAMOND = 4, 
    //##Documentation
    //## ��Բ�ͼ�ͷ
    GLA_OVAL    = 5, 
    //##Documentation
    //## ʵ�����
    GLAF_SOLID = 0x0000,
	//##Documentation
    //## �������
    GLAF_EMPTY = 0x1000 };

//##ModelId=436C63FF0280
//##Documentation
//## ���壺
//##        ����ģʽ����
//## ���ܣ�
//##        ���屳��ģʽ
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
enum  { 
    //##Documentation
    //## ����ͼ���С
    GFMD_LOCK, 
    //##Documentation
    //## ����
    GFMD_CENTER,
    //##Documentation
    //## ƽ��
    GFMD_TILE,
    //##Documentation
    //## ����
    GFMD_STRETCH,
	//##Documentation
    //## ����ƽ��-�����߶�
	GFMDT_HORZ,
	//##Documentation
    //## ����ƽ��-�������
	GFMDT_VERT,
	//##Documentation
	//##ͼ����Դ�ļ�
	GFMD_FILE=0x00,
	//##Documentation
	//##ͼ����Դ������Ƕ
	GFMD_DATA=0x01};

//##ModelId=436C7524001F
//##Documentation
//## ���壺
//##       ͼԪ�ڲ��������붨��
//## ���ܣ�
//##       ����ͼԪ�ڲ���������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
enum  { 
    //##Documentation
    //## �ڲ�����ʹ���߷��ʱ
    //##    (getStyleMask()&GBOM_LINESTYLE)==1
    GBOM_LINESTYLE = 0x01, 
    //##Documentation
    //## �ڲ�����ʹ�������ʱ
    //##    (getStyleMask()&GBOM_FILLSTYLE)==1
    GBOM_FILLSTYLE = 0x02, 
    //##Documentation
    //## �ڲ�����ʹ���ı����ʱ
    //##    (getStyleMask()&GBOM_TEXTSTYLE)==1
    GBOM_TEXTSTYLE = 0x04, 
    //##Documentation
    //## �ڲ�����ʹ�ÿ��ӷ�Χʱ
    //##    (getStyleMask()&GBOM_RANGE)==1
    GBOM_RANGE = 0x08, 
    //##Documentation
    //## �ڲ�����ʹ���ڲ���������ʱ
    //##    (getStyleMask()&GBOM_SCALE)==1
    GBOM_SCALE = 0x10, 
    //##Documentation
    //## �ڲ�����ʹ���ڲ���ת����ʱ
    //##    (getStyleMask()&GBOM_ROTATE)==1
    GBOM_ROTATE = 0x20 , 
    //##Documentation
    //## �ڲ�����ʹ���ڲ���������ʱ
    //##    (getStyleMask()&GBOM_DATA)==1
    GBOM_DATA = 0x40 ,	
	//##Documentation
	//## �ڲ�����ʹ����չ���ʱ
    //##    (getStyleMask()&GBOM_DEVTYPE)==1
	GBOM_DEVTYPE = 0x80,
	//##Documentation
	//## �ڲ�����ʹ����չ���ʱ
    //##    (getStyleMask()&GBOM_DEVSTATUS)==1
	GBOM_DEVSTATUS = 0x100,
	//##Documentation
	//## �ڲ�����,����ͼԪ����
    //##    (getStyleMask()&GBOM_UPDATE)==1
	GBOM_UPDATE = 0x10000000 };

//##ModelId=436C9F1D036B
//##Documentation
//## ���壺
//##       �������붨��
//## ���ܣ�
//##        ������������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
enum  { 
    //##Documentation
    //## ����
    GSFM_SCALE = 1, 
    //##Documentation
    //## ��ת
    GSFM_ROTATE= 2 };

//##ModelId=436C53240222
//##Documentation
//## ���壺
//##        ������붨��
//## ���ܣ�
//##        �����������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
enum  { 
    //##Documentation
    //## ͸����ʾ
    GFM_TRANS = 0, 
    //##Documentation
    //## �����ɫ���洢��ɫ
    GFM_SOLID = 1, 
    //##Documentation
    //## ��դ���
    GFM_RASTER = 2,
    //##Documentation
    //## ���ģʽ���洢ģʽ
    GFM_PATTERN = 3, 
    //##Documentation
    //## ���ͼ�񣬴洢ͼ��
    GFM_IMAGE = 4 };

//##ModelId=436D785E0109
//##Documentation
//## ���壺
//##        ��դ������붨��
//## ���ܣ�
//##        �����դ�������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-5
enum  { 
   //##Documentation
    //## �����
    GFMR_SIMPLE = 0x00, 
    //##Documentation
    //## ˫ɫ����
    GFMR_SHADE = 0x01, 
    //##Documentation
    //## ˫ɫ����
    GFMR_EXSHADE = 0x02, 
    //##Documentation
    //## GL����ģʽ���
    GFMR_GLMF = 0x03, 
	//�Ƕ����
	GFMR_CONICAL = 0x04,

	//�������
	GFMR_RADIAL = 0x05,

	//����4��Ϊ�������ķ���
    //##Documentation
    //## �������
    GFMR_HORZ = 0x00, 
    //##Documentation
    //## �������
    GFMR_VERT = 0x01,
	//##Documentation
	//## �Զ��������
	//## ���W>H����������
	//## ���W<H�����������
	GFMR_AUTO = 0x02,

	//��б�����
	GFMR_SLASH = 0x03,
	//��б�����
	GFMR_BACKSLASH = 0x04,

	//����4��Ϊ�Ƕ����Ŀ�ѡ�Ƕ�
	//0��
	GFMR_0 = 0x04,
	//90��
	GFMR_90 = 0x05,
	//180��
	GFMR_180 = 0x06,
	//270��
	GFMR_270 = 0x07
     };


//##ModelId=436DB3B60232
//##Documentation
//## ���壺
//##        ���������붨��
//## ���ܣ�
//##        ��������������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-6
enum  { 
   //##Documentation
    //## ��������
    GTFS_NORMAL = 0x00, 
    //##Documentation
    //## �Ӵ�����
    GTFS_BOLD = 0x01, 
    //##Documentation
    //## ��б����
    GTFS_ITALIC = 0x100, 
    //##Documentation
    //## �»���
    GTFS_UNDERLINE = 0x200, 
    //##Documentation
    //## �ϻ���
    GTFS_OVERLINE = 0x400, 
    //##Documentation
    //## ɾ����
    GTFS_STRIKEOUT = 0x800, 
    //##Documentation
    //## ��Ӱ
    GTFS_SHADOW = 0x1000,
	//##Documentation
    //## �߿�
    GTFS_FRAME	= 0x2000 ,
	//## Documentation
    //## ��������
	GTFS_VERTICAL= 0x4000,
	//## Documentation
    //## ��������
	GTFS_AUTOSIZE= 0x8000 };

//##ModelId=436DB53E01F4
//##Documentation
//## ���壺
//##        ����������붨��
//## ���ܣ�
//##        ���������������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-6
enum  { 
    //##Documentation
    //## �������
    GTFA_LEFT = 0x01, 
    //##Documentation
    //## ���ж���
    GTFA_CENTER = 0x02, 
    //##Documentation
    //## ���Ҷ���
    GTFA_RIGHT = 0x04, 
    //##Documentation
    //## ���϶���
    GTFA_TOP = 0x10, 
    //##Documentation
    //## ��ֱ���ж���
    GTFA_VCENTER = 0x20, 
    //##Documentation
    //## ���¶���
    GTFA_BOTTOM = 0x40, 
    //##Documentation
    //## �Զ�����
    GTFA_WRAP = 0x100 };


//##ModelId=436DFBC7000F
//##Documentation
//## ���壺
//##        ���屳��ģʽ���붨��
//## ���ܣ�
//##        �������屳��ģʽ����
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-6
enum  { 
    //##Documentation
    //## ͸����ʾ
    GTM_TRANS = 0x00, 
    //##Documentation
    //## ͸����ʾ
    GTM_TRANSPARENT = 0x00, 
    //##Documentation
    //## ��͸����
    GTM_OPAQUE = 0x01 };

//##ModelId=4376F21200FA
//##Documentation
//## ���壺
//##        ����ͼԪ��ϵ
//## ���ܣ�
//##        ����ͼԪ��ϵ
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-13
enum  { 
    //##Documentation
    //## �ƶ������������ƶ�����
    GHT_MOVE = 0x000001, 
    //##Documentation
    //## �ߴ����
    GHT_SIZE = 0x000002, 
    //##Documentation
    //## ��ת���ĵ�
    GHT_ROTATE = 0x000004,
    //##Documentation
    //## ����ѡ��״̬,
	//## ���ھ����������ѡ��
    GHT_SELECT = 0x000008,
    //##Documentation
    //## ˮƽ�������
    GHTC_SIZEHL = 0x000010, 
	 //##Documentation
    //## ˮƽ�����ұ�
    GHTC_SIZEHR = 0x000020, 
    //##Documentation
    //## ��ֱ�����ϱ�
    GHTC_SIZEVT = 0x000040, 
	//##Documentation
    //## ��ֱ�����±�
    GHTC_SIZEVB = 0x000080, 
    //##Documentation
    //## ���ҵ�����
    GHTC_SIZEBT = 0x000100, 
	 //##Documentation
    //## ���������
    GHTC_SIZEBB = 0x000200, 
	 //##Documentation
    //## ���������
    GHTC_SIZEFT = 0x000400, 
    //##Documentation
    //## ���ҵ�����
    GHTC_SIZEFB = 0x000800, 

    //##Documentation
    //## ��ʼԼ��
    GHTH_OS = 0x001000, 
    //##Documentation
    //## ��ֹԼ��
    GHTH_OE = 0x002000, 
    //##Documentation
    //## ����Լ��
    GHTH_OC = 0x004000, 
    //##Documentation
    //## �ڲ�����Լ��������ñ�־Ϊ��
    //## ��ͼԪ���������Ϣ
    GHTH_OI = 0x008000,
	//##Documentation
    //## ���Ե��ѡ��Լ��
	GHTH_SELECT=0x010000};

//##ModelId=43793A3E035B
//##Documentation
//## ���壺
//##      ���ƽ�������ѡ��
//## ���ܣ�
//##      ���ƽ�������ѡ��
//## ��ƣ�
//##      ������
//## ���ڣ�
//##      2005-11-15
enum  { 
    //##Documentation
    //## ������ת���ĵ�
    GDF_CENTER = 0x0001, 
    //##Documentation
    //## ʹ����ת�����
    GDF_ROTATE = 0x0002, 
    //##Documentation
    //## ���Ʋ�����
    GDF_OPERATOR = 0x0004, 
    //##Documentation
    //## ���Ʊ߿�
    GDF_FRAME = 0x0008 , 
    //##Documentation
    //## ������껬��
    GDF_OVER = 0x0010 ,
	//##Documentation
    //## �ȵ㻬��
	GDF_HOT  =0x0020};


//##ModelId=437937B700DA
//##Documentation
//## ���壺
//##       ͼԪ�����������
//## ���ܣ�
//##       ����ͼԪ�������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-15
enum  { 
    //##Documentation
    //## ��X��Ϊ�Գ��ᣬ���о���
    GMF_X = 0x01, 
    //##Documentation
    //## ��Y��Ϊ�Գ��ᣬ���о���
    GMF_Y = 0x02, 
    //##Documentation
    //## ��O��Ϊ�Գ��ᣬ���о���
    GMF_O = 0x03, 
    //##Documentation
    //## ��Y=X��Ϊ�Գ��ᣬ���о���
    GMF_XY = 0x04, 
    //##Documentation
    //## ��Y=-XΪ�Գ��ᣬ���о���
    GMF_NXY = 0x05 };

//##ModelId=437ED1D2036B
//##Documentation
//## ���壺
//##       ͼ�����Ͷ���
//## ���ܣ�
//##       ����ͼ������
//## ��ƣ�
//##       ������
//## ���ڣ�
//##       2005-11-19
enum  { 
    //##Documentation
    //## ����㣭���������빲���������ͬʱ��ʾ
    //##         �����������ͬʱ��ʾ
    GLT_SHARE = 0x0000, 
    //##Documentation
    //## ����㣭����㲻���뻥��㡢������ͬʱ��ʾ
    //##         �����빲���ͬʱ��ʾ
    //##         
    GLT_MUTEX = 0x0001, 
    //##Documentation
    //## �����㣭������������ͬʱ��ʾ
    GLT_SELF = 0x0002, 
    //##Documentation
    //## ���ز㣭�ò㲻��ʾ
    GLT_HIDE = 0x0003, 
    //##Documentation
    //## ����ͼ�㣭���ڸ�״̬��ͼ�β��ܱ��༭��ֻ�����
    //##           ��ͬʱ��ʾ��ͼ����ʾ
    GLT_LOCK = 0x00010000, 
    //##Documentation
    //## �ر�ͼ�㣭���ڸ�״̬��ͼ�㲻��ʾ�������༭
    //##           ֱ����ͼ�㱻��
    GLT_CLOSE = 0x00020000 };
//##ModelId=4387F8BC00BB
//##Documentation
//## ���ܣ�
//##    ͼԪ����ѡ����ڶ���ͼԪ
//## ���壺
//##    ����ͼԪ����ѡ��
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum  { 
    //##Documentation
    //## �����
    GBA_LEFT = 0x01, 
    //##Documentation
    //## ˮƽ���ж���
    GBA_CENTER = 0x02, 
    //##Documentation
    //## �Ҷ���
    GBA_RIGHT = 0x04, 
    //##Documentation
    //## �϶���
    GBA_TOP = 0x08, 
    //##Documentation
    //## �¶���
    GBA_BOTTOM = 0x10, 
    //##Documentation
    //## ��ֱ����
    GBA_VCENTER = 0x20 };

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ͼԪ�ȳߴ�ѡ����ڵȳߴ�ͼԪ
//## ���壺
//##    ����ͼԪ�ȳߴ�ѡ��
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum  { 
    //##Documentation
    //## �ȿ�
    GBS_WIDTH = 0x01, 
    //##Documentation
    //## �ȸ�
    GBS_HEIGHT = 0x02, 
    //##Documentation
    //## �ȿ��
    GBS_BOTH = 0x04 };

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ��ť͹�𼰰���
//## ���壺
//##    ���尴ť͹�𼰰���
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum  {
	//##Documentation
    //## ͹��
	GDBS_RAISED	  = 0x010000,
	//##Documentation
    //## ����
	GDBS_SUNKEN	  = 0x020000};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ��ť������
//## ���壺
//##    ��ť������
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum  {
	//##Documentation
    //## �л�ͼ�� 
	btnHotFile,
	//##Documentation
    //## ����ͼ�� 
	btnHotCommand,
	//##Documentation
    //## ���ù���
	btnHotMacro,
	//##Documentation
	//## �л�ͼ��
	btnHotLayer};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ��̬ͼԪ��ʾģʽ����
//## ���壺
//##    ��̬ͼԪ��ʾģʽ����
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum  { 
	//##Documentation
    //## NORMAL��ʾģʽ
	GDT_NORMAL= 0x01,
    //##Documentation
    //## LCD��ʾģʽ
    GDT_LCD   = 0x02, 
	//##Documentation
    //## ��ͷ��ʾģʽ
	GDT_ARROW = 0x04,
    //##Documentation
    GDT_SIGN = 0x08,
    //## �ϼ�ͷ
    GDTA_UP   = 0x10, 
    //##Documentation
    //## �¼�ͷ
    GDTA_DOWN = 0x20,
	//##Documentation
    //## ���ͷ
    GDTA_LEFT = 0x40, 
    //##Documentation
    //## �Ҽ�ͷ
    GDTA_RIGHT= 0x80,
	//##Documentation
    //## �޼�ͷ
    GDTA_MASK = 0xF0};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ͼԪ��Ϊ����
//## ���壺
//##    ͼԪ��Ϊ����
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum  {
	//##Documentation
    //## ������
	GOAM_NONE=0x00,
	//##Documentation
    //## ��˸
	GOAM_FLASH=0x01,
	//##Documentation
    //## ��ǰ��ɫ
	GOAM_FCLR=0x02,
	//##Documentation
    //## �䱳��ɫ
	GOAM_BCLR=0x04,
	//##Documentation
    //## �߿�ɫ
	GOAM_LCLR=0x08,
	//##Documentation
    //## ����ɫ
	GOAM_TCLR=0x10,
	//##Documentation
    //## ���ֱ���ɫ
	GOAM_TBCLR=0x20,
	//##Documentation
    //## �Զ���ת
	GOAM_ROTATE=0x40 };

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ͼԪ��Ϊ����
//## ���壺
//##    ͼԪ��Ϊ����
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum {
	//##Documentation
    //## ����˸
	GOAMF_NONE,
	//##Documentation
    //## �ɼ��벻�ɼ���˸
	GOAMF_VISIBLE,
	//##Documentation
    //## ǰ���뱳��������˸
	GOAMF_INVERT,
	//##Documentation
    //## �߿������˸
	GOAMF_FRAME   ,
	//##Documentation
    //## ����벻�����˸
	GOAMF_FILL};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ���߻���ģʽ
//## ���壺
//##    ���߻���ģʽ
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum {
	//##Documentation
    //## ����
	GPAM_ARC,
	//##Documentation
    //## ����
	GPAM_CHD,
	//##Documentation
    //## ����
	GPAM_PIE};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ·����������
//## ���壺
//##    ·����������
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    20065-06-19
enum 
{
	//##Documentation
    //## �����ƶ���ĳ��
	GPH_MOVETO  =0x06,
	//##Documentation
    //## ���Ʒ����
	GPH_CLOSE	=0x01,
	//##Documentation
    //## �������Ƶ�ĳ��
	GPH_LINETO  =0x02,
	//##Documentation
    //## ���������߻��Ƶ�
	GPH_CURVETO	=0x04,
	//##Documentation
    //## ���߻��Ƶ�
	GPH_ARCTO	=0x08};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ���߻���ģʽ
//## ���壺
//##    ���߻���ģʽ
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum {
	////##Documentation
    //## ����
	GPM_LINES=0x00,
	//##Documentation
    //## ���
	GPM_CLOSE=0x01,
	//##Documentation
    //## Windingģʽ
	GPM_WINDING=0x02,
	//##Documentation
    //## ��żģʽ
	GPM_EVENODD=0x04};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ����ת��
//## ���壺
//##    ����ת��
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum {
	//##Documentation
    //## �ȱ�����
	COOR_SCALE,
	//##Documentation
    //## ���ſ��
	COOR_SCALEW,
	//##Documentation
    //## ���Ÿ߶�
	COOR_SCALEH};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ����״̬
//## ���壺
//##    ����״̬
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
//## Ӧ��������ͼԪʹ��m_extFlag����
//## ��Ӧ����������
//## ����
//##	���ֽڱ�ʶ���Ӹ���
//##	���ֽڱ�ʶ�Ƿ�����
//## ����
//##	���ֽڱ�ʶ���Ӹ���
//##	���ֽڱ�ʶ�Ƿ�����
//## ��Ӧĸ�ߺͶ���
//## ֻʹ�õ��ֺ��Ը���
//## ����
//##	���ֽڱ�ʶ���Ӹ���
//##	���ֽڱ�ʶ�Ƿ�����

enum {
	//##Documentation
    //## ���ַ���-��Ӧĸ�߶���
	LO_IGNORE=0x00,
	//##Documentation
    //## �����ӣ���Ӧ������
	LO_START =0x01,
	//##Documentation
    //## β���ӣ���Ӧ������
	LO_END   =0x02,
	//##Documentation
    //## һ�����Ӳ���
	LO_LINK  =0x03,
	//##Documentation
    //## ����ʧ��
	LS_FAILED=0x00,
	//##Documentation
    //## ���ӳɹ�
	LS_LINK	 =0x01};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ͼԪ�����Ӧ
//## ���壺
//##    ͼԪ�����Ӧ
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum
{
	//##Documentation
	//## ����Ҽ�
	GMM_RBTN  = 0x00,
	//##Documentation
	//## ������
	GMM_LBTN  = 0x01,
	//##Documentation
	//## ����ƶ�
	GMM_MOUSE = 0x02,
	//##Documentation
	//## ����
	GMM_DOWN  = 0x00,
	//##Documentation
	//## ����
	GMM_UP	  = 0x01,
	//##Documentation
	//## ����
	GMM_ENTER = 0x00,
	//##Documentation
	//## �˳�
	GMM_LEAVE = 0x01,
	//##Documentation
	//## �ƶ�
	GMM_MOVE  = 0x02
};

//##ModelId=4387F9CC007D
//##Documentation
//## ���ܣ�
//##    ͼ�����Ź���
//## ���壺
//##    ͼ�����Ź���
//## ��ƣ�
//##    ������
//## ���ڣ�
//##    2005-11-26
enum
{
	//##Documentation
	//## ԭͼ��ʾ
	GFR_NORMAL,
	//##Documentation
	//## �Զ�����
	GFR_AUTOSIZE,
	//##Documentation
	//## ��ȷ���
	GFR_AUTOWIDTH,
	//##Documentation
	//## �߶ȷ���
	GFR_AUTOHEIGHT
};

#endif /* GOBJECTCOMMON_H_HEADER_INCLUDED_BC939898 */
