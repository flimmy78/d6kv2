#include "rdb_table.h"
#include <QHeaderView>

Rdb_table::Rdb_table( QWidget* parent /*= 0 */ )
{
	QTableWidget::setSortingEnabled( true ) ;
	setEditTriggers( QAbstractItemView::NoEditTriggers ) ;
// 	QHeaderView* pV_headerview = this->verticalHeader() ;
// 	pV_headerview->setResizeMode( QHeaderView::Stretch ) ;
// 	QHeaderView* pH_headerview = this->horizontalHeader() ;
// 	pH_headerview->setResizeMode( QHeaderView::Stretch ) ;
}

void Rdb_table::paintCell( int row , int col , const QBrush & brush )
{
	QString item_text = item( row , col )->text() ;
	QBrush g( brush );

	if( col >2)
	{
		if( item_text == tr("��") || item_text == tr("����") || item_text == tr("����") || item_text ==tr("����") )
		{	
			g.setColor( Qt::red );
		}
	}
	item( row , col )->setForeground( g ) ;
}
