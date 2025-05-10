/****************************************************************************
** Meta object code from reading C++ file 'chatwidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../chatwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSChatWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSChatWidgetENDCLASS = QtMocHelpers::stringData(
    "ChatWidget",
    "chatloginout",
    "",
    "on_groups_clicked",
    "on_friends_clicked",
    "on_infowidget_clicked",
    "std::shared_ptr<Group>",
    "group",
    "std::shared_ptr<User>",
    "user",
    "on_send_clicked",
    "handleChatMessage",
    "json",
    "js",
    "handleGroupMessage",
    "on_user_clicked",
    "on_loginout_clicked",
    "on_add_friend_clicked",
    "handleAddFriendRequest",
    "handleAddFriendResponse",
    "handleUpdateFriendResponse",
    "handleCreateGroupResponse",
    "handleUpdateGroupResponse",
    "handleAddGroupRequest",
    "handleAddGroupResponse",
    "on_letters_clicked",
    "on_create_group_clicked",
    "on_addgroup_clicked",
    "on_picture_clicked",
    "on_upload_file_clicked",
    "on_quit_group_clicked",
    "on_remove_group_clicked",
    "on_publish_inform_clicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSChatWidgetENDCLASS_t {
    uint offsetsAndSizes[66];
    char stringdata0[11];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[19];
    char stringdata5[22];
    char stringdata6[23];
    char stringdata7[6];
    char stringdata8[22];
    char stringdata9[5];
    char stringdata10[16];
    char stringdata11[18];
    char stringdata12[5];
    char stringdata13[3];
    char stringdata14[19];
    char stringdata15[16];
    char stringdata16[20];
    char stringdata17[22];
    char stringdata18[23];
    char stringdata19[24];
    char stringdata20[27];
    char stringdata21[26];
    char stringdata22[26];
    char stringdata23[22];
    char stringdata24[23];
    char stringdata25[19];
    char stringdata26[24];
    char stringdata27[20];
    char stringdata28[19];
    char stringdata29[23];
    char stringdata30[22];
    char stringdata31[24];
    char stringdata32[26];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSChatWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSChatWidgetENDCLASS_t qt_meta_stringdata_CLASSChatWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "ChatWidget"
        QT_MOC_LITERAL(11, 12),  // "chatloginout"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 17),  // "on_groups_clicked"
        QT_MOC_LITERAL(43, 18),  // "on_friends_clicked"
        QT_MOC_LITERAL(62, 21),  // "on_infowidget_clicked"
        QT_MOC_LITERAL(84, 22),  // "std::shared_ptr<Group>"
        QT_MOC_LITERAL(107, 5),  // "group"
        QT_MOC_LITERAL(113, 21),  // "std::shared_ptr<User>"
        QT_MOC_LITERAL(135, 4),  // "user"
        QT_MOC_LITERAL(140, 15),  // "on_send_clicked"
        QT_MOC_LITERAL(156, 17),  // "handleChatMessage"
        QT_MOC_LITERAL(174, 4),  // "json"
        QT_MOC_LITERAL(179, 2),  // "js"
        QT_MOC_LITERAL(182, 18),  // "handleGroupMessage"
        QT_MOC_LITERAL(201, 15),  // "on_user_clicked"
        QT_MOC_LITERAL(217, 19),  // "on_loginout_clicked"
        QT_MOC_LITERAL(237, 21),  // "on_add_friend_clicked"
        QT_MOC_LITERAL(259, 22),  // "handleAddFriendRequest"
        QT_MOC_LITERAL(282, 23),  // "handleAddFriendResponse"
        QT_MOC_LITERAL(306, 26),  // "handleUpdateFriendResponse"
        QT_MOC_LITERAL(333, 25),  // "handleCreateGroupResponse"
        QT_MOC_LITERAL(359, 25),  // "handleUpdateGroupResponse"
        QT_MOC_LITERAL(385, 21),  // "handleAddGroupRequest"
        QT_MOC_LITERAL(407, 22),  // "handleAddGroupResponse"
        QT_MOC_LITERAL(430, 18),  // "on_letters_clicked"
        QT_MOC_LITERAL(449, 23),  // "on_create_group_clicked"
        QT_MOC_LITERAL(473, 19),  // "on_addgroup_clicked"
        QT_MOC_LITERAL(493, 18),  // "on_picture_clicked"
        QT_MOC_LITERAL(512, 22),  // "on_upload_file_clicked"
        QT_MOC_LITERAL(535, 21),  // "on_quit_group_clicked"
        QT_MOC_LITERAL(557, 23),  // "on_remove_group_clicked"
        QT_MOC_LITERAL(581, 25)   // "on_publish_inform_clicked"
    },
    "ChatWidget",
    "chatloginout",
    "",
    "on_groups_clicked",
    "on_friends_clicked",
    "on_infowidget_clicked",
    "std::shared_ptr<Group>",
    "group",
    "std::shared_ptr<User>",
    "user",
    "on_send_clicked",
    "handleChatMessage",
    "json",
    "js",
    "handleGroupMessage",
    "on_user_clicked",
    "on_loginout_clicked",
    "on_add_friend_clicked",
    "handleAddFriendRequest",
    "handleAddFriendResponse",
    "handleUpdateFriendResponse",
    "handleCreateGroupResponse",
    "handleUpdateGroupResponse",
    "handleAddGroupRequest",
    "handleAddGroupResponse",
    "on_letters_clicked",
    "on_create_group_clicked",
    "on_addgroup_clicked",
    "on_picture_clicked",
    "on_upload_file_clicked",
    "on_quit_group_clicked",
    "on_remove_group_clicked",
    "on_publish_inform_clicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSChatWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  170,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,  171,    2, 0x08,    2 /* Private */,
       4,    0,  172,    2, 0x08,    3 /* Private */,
       5,    1,  173,    2, 0x08,    4 /* Private */,
       5,    1,  176,    2, 0x08,    6 /* Private */,
      10,    0,  179,    2, 0x08,    8 /* Private */,
      11,    1,  180,    2, 0x08,    9 /* Private */,
      14,    1,  183,    2, 0x08,   11 /* Private */,
      15,    1,  186,    2, 0x08,   13 /* Private */,
      16,    0,  189,    2, 0x08,   15 /* Private */,
      17,    0,  190,    2, 0x08,   16 /* Private */,
      18,    1,  191,    2, 0x08,   17 /* Private */,
      19,    1,  194,    2, 0x08,   19 /* Private */,
      20,    1,  197,    2, 0x08,   21 /* Private */,
      21,    1,  200,    2, 0x08,   23 /* Private */,
      22,    1,  203,    2, 0x08,   25 /* Private */,
      23,    1,  206,    2, 0x08,   27 /* Private */,
      24,    1,  209,    2, 0x08,   29 /* Private */,
      25,    0,  212,    2, 0x08,   31 /* Private */,
      26,    0,  213,    2, 0x08,   32 /* Private */,
      27,    0,  214,    2, 0x08,   33 /* Private */,
      28,    0,  215,    2, 0x08,   34 /* Private */,
      29,    0,  216,    2, 0x08,   35 /* Private */,
      30,    0,  217,    2, 0x08,   36 /* Private */,
      31,    0,  218,    2, 0x08,   37 /* Private */,
      32,    0,  219,    2, 0x08,   38 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ChatWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSChatWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSChatWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSChatWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ChatWidget, std::true_type>,
        // method 'chatloginout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_groups_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_friends_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_infowidget_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<Group>, std::false_type>,
        // method 'on_infowidget_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<User>, std::false_type>,
        // method 'on_send_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleChatMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'handleGroupMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'on_user_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<User>, std::false_type>,
        // method 'on_loginout_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_add_friend_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleAddFriendRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'handleAddFriendResponse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'handleUpdateFriendResponse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'handleCreateGroupResponse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'handleUpdateGroupResponse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'handleAddGroupRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'handleAddGroupResponse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const json &, std::false_type>,
        // method 'on_letters_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_create_group_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_addgroup_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_picture_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_upload_file_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_quit_group_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_remove_group_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_publish_inform_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ChatWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->chatloginout(); break;
        case 1: _t->on_groups_clicked(); break;
        case 2: _t->on_friends_clicked(); break;
        case 3: _t->on_infowidget_clicked((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<Group>>>(_a[1]))); break;
        case 4: _t->on_infowidget_clicked((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<User>>>(_a[1]))); break;
        case 5: _t->on_send_clicked(); break;
        case 6: _t->handleChatMessage((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 7: _t->handleGroupMessage((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 8: _t->on_user_clicked((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<User>>>(_a[1]))); break;
        case 9: _t->on_loginout_clicked(); break;
        case 10: _t->on_add_friend_clicked(); break;
        case 11: _t->handleAddFriendRequest((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 12: _t->handleAddFriendResponse((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 13: _t->handleUpdateFriendResponse((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 14: _t->handleCreateGroupResponse((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 15: _t->handleUpdateGroupResponse((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 16: _t->handleAddGroupRequest((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 17: _t->handleAddGroupResponse((*reinterpret_cast< std::add_pointer_t<json>>(_a[1]))); break;
        case 18: _t->on_letters_clicked(); break;
        case 19: _t->on_create_group_clicked(); break;
        case 20: _t->on_addgroup_clicked(); break;
        case 21: _t->on_picture_clicked(); break;
        case 22: _t->on_upload_file_clicked(); break;
        case 23: _t->on_quit_group_clicked(); break;
        case 24: _t->on_remove_group_clicked(); break;
        case 25: _t->on_publish_inform_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChatWidget::*)();
            if (_t _q_method = &ChatWidget::chatloginout; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *ChatWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSChatWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ChatWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void ChatWidget::chatloginout()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
