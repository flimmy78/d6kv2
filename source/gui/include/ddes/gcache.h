#ifndef _GCACHE_H
#define _GCACHE_H

#include "ddef/GList.h"

class QPixmap;
class QLibrary;
class GBaseObject;
class GGraphDynData;

typedef GBaseObject* (*GDOFB_PROC)();
typedef GGraphDynData* (*GDDFB_PROC)();

//ͼ�񻺳�
typedef struct __tagImageCache
{
	QString		strPath;	//·��
	uint32		lastTime;	//���ʹ��ʱ��
	QPixmap*	qpixmap;	//ͼƬ����
}IMAGECACHE,*LPIMAGECACHE;

//ͼ������
typedef struct __tagSymbolCache
{
	QString			strPath;	//·��
	uint32			nID    ;	//��ʶ
	uint32			lastTime;	//���ʹ��ʱ��
	GBaseObject*	symbol;		//ͼ������
}SYMBOLCACHE,*LPSYMBOLCACHE;

//�������
typedef struct __tagPluginCache
{
	QString			strPath;	//·��
	uint32			lastTime;	//���ʹ��ʱ��
	GDOFB_PROC		pluginObj;	//�������
	QLibrary*		library;
}PLUGINCACHE,*LPPLUGINCACHE;

typedef struct __tagDynDataCache
{
	QString			strPath;
	uint32			lastTime;
	GDDFB_PROC		dynData;
	QLibrary*		library;
}DYNDATACACHE,*LPDYNDATACACHE;

class GDE_EXPORT_DES GListIMG:public GList<LPIMAGECACHE>{};
class GDE_EXPORT_DES GListSYM:public GList<LPSYMBOLCACHE>{};
class GDE_EXPORT_DES GListPLG:public GList<LPPLUGINCACHE>{};
class GDE_EXPORT_DES GListDYN:public GList<LPDYNDATACACHE>{};

class GDE_EXPORT_DES GImageCache
{
public:
	GImageCache();
	~GImageCache();

	void   setMaxSize(uint32 size);
	uint32  getMaxSize() const;
	uint32  size() const;

	void		 add(const char* strPath,QPixmap* qpixmap);
	LPIMAGECACHE find(const char* strPath);

	void clear();

protected:

	void removeOlder();

protected:

	uint32		m_maxSize;
	GListIMG	m_cache  ;
};

class GDE_EXPORT_DES GSymbolCache
{
public:
	GSymbolCache();
	~GSymbolCache();

	void   setMaxSize(uint32 size);
	uint32  getMaxSize() const;
	uint32  size() const;

	void		  add(const char* strPath,uint32 nID,GBaseObject* symbol);
	LPSYMBOLCACHE find(const char* strPath,uint32 nID);

	void clear();

protected:

	void removeOlder();

protected:

	uint32		m_maxSize;
	GListSYM	m_cache  ;
};

class GDE_EXPORT_DES GPluginCache
{
public:
	GPluginCache();
	~GPluginCache();

	void   setMaxSize(uint32 size);
	uint32  getMaxSize() const;
	uint32  size() const;

	void		  add(const char* strPath,QLibrary *lib,GDOFB_PROC pluginObj);
	LPPLUGINCACHE find(const char* strPath);

	void clear();

protected:

	void removeOlder();

protected:

	uint32		m_maxSize;
	GListPLG	m_cache  ;
};

class GDE_EXPORT_DES GDynDataCache
{
public:
	GDynDataCache();
	~GDynDataCache();

	void   setMaxSize(uint32 size);
	uint32  getMaxSize() const;
	uint32  size() const;

	void		  add(const char* strPath,QLibrary* lib,GDDFB_PROC dynData);
	LPDYNDATACACHE find(const char* strPath);

	void clear();

protected:

	void removeOlder();

protected:

	uint32		m_maxSize;
	GListDYN	m_cache  ;
};

#endif
