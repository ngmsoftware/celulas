#include "teste.moc"
/****************************************************************************
** base meta object code from reading C++ file 'teste.moc'
**
** Created: Thu Nov 29 12:47:02 2001
**      by: The Qt MOC ($Id: qt/src/moc/moc.y   2.2.1   edited 2000-09-29 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_base
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 9
#elif Q_MOC_OUTPUT_REVISION != 9
#error "Moc format conflict - please regenerate all moc files"
#endif

#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *base::className() const
{
    return "base";
}

QMetaObject *base::metaObj = 0;

void base::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("base","QWidget");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString base::tr(const char* s)
{
    return qApp->translate( "base", s, 0 );
}

QString base::tr(const char* s, const char * c)
{
    return qApp->translate( "base", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* base::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QWidget::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(base::*m1_t0)();
    typedef void(base::*m1_t1)();
    typedef void(base::*m1_t2)();
    typedef void(base::*m1_t3)();
    typedef void(base::*m1_t4)();
    typedef void(base::*m1_t5)();
    typedef void(base::*m1_t6)();
    m1_t0 v1_0 = Q_AMPERSAND base::SaveImage;
    m1_t1 v1_1 = Q_AMPERSAND base::OpenImage;
    m1_t2 v1_2 = Q_AMPERSAND base::do_it_slot;
    m1_t3 v1_3 = Q_AMPERSAND base::do_it21_slot;
    m1_t4 v1_4 = Q_AMPERSAND base::do_it22_slot;
    m1_t5 v1_5 = Q_AMPERSAND base::do_it3_slot;
    m1_t6 v1_6 = Q_AMPERSAND base::do_it4_slot;
    QMetaData *slot_tbl = QMetaObject::new_metadata(7);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(7);
    slot_tbl[0].name = "SaveImage()";
    slot_tbl[0].ptr = (QMember)v1_0;
    slot_tbl_access[0] = QMetaData::Public;
    slot_tbl[1].name = "OpenImage()";
    slot_tbl[1].ptr = (QMember)v1_1;
    slot_tbl_access[1] = QMetaData::Public;
    slot_tbl[2].name = "do_it_slot()";
    slot_tbl[2].ptr = (QMember)v1_2;
    slot_tbl_access[2] = QMetaData::Public;
    slot_tbl[3].name = "do_it21_slot()";
    slot_tbl[3].ptr = (QMember)v1_3;
    slot_tbl_access[3] = QMetaData::Public;
    slot_tbl[4].name = "do_it22_slot()";
    slot_tbl[4].ptr = (QMember)v1_4;
    slot_tbl_access[4] = QMetaData::Public;
    slot_tbl[5].name = "do_it3_slot()";
    slot_tbl[5].ptr = (QMember)v1_5;
    slot_tbl_access[5] = QMetaData::Public;
    slot_tbl[6].name = "do_it4_slot()";
    slot_tbl[6].ptr = (QMember)v1_6;
    slot_tbl_access[6] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
	"base", "QWidget",
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}


const char *MyDialog::className() const
{
    return "MyDialog";
}

QMetaObject *MyDialog::metaObj = 0;

void MyDialog::initMetaObject()
{
    if ( metaObj )
	return;
    if ( qstrcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("MyDialog","QDialog");
    (void) staticMetaObject();
}

#ifndef QT_NO_TRANSLATION

QString MyDialog::tr(const char* s)
{
    return qApp->translate( "MyDialog", s, 0 );
}

QString MyDialog::tr(const char* s, const char * c)
{
    return qApp->translate( "MyDialog", s, c );
}

#endif // QT_NO_TRANSLATION

QMetaObject* MyDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    (void) QDialog::staticMetaObject();
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    typedef void(MyDialog::*m1_t0)();
    m1_t0 v1_0 = Q_AMPERSAND MyDialog::myaccept;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    QMetaData::Access *slot_tbl_access = QMetaObject::new_metaaccess(1);
    slot_tbl[0].name = "myaccept()";
    slot_tbl[0].ptr = (QMember)v1_0;
    slot_tbl_access[0] = QMetaData::Public;
    metaObj = QMetaObject::new_metaobject(
	"MyDialog", "QDialog",
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    metaObj->set_slot_access( slot_tbl_access );
#ifndef QT_NO_PROPERTIES
#endif // QT_NO_PROPERTIES
    return metaObj;
}
