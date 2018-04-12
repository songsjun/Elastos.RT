#ifndef __CAR_CHAT_H__
#define __CAR_CHAT_H__

#ifndef _NO_INCLIST
#include <elastos.h>
using namespace Elastos;
#include <Elastos.CoreLibrary.h>


#endif // !_NO_INCLIST


interface IMessageListener;
EXTERN const _ELASTOS InterfaceID EIID_IMessageListener;
interface IChat;
EXTERN const _ELASTOS InterfaceID EIID_IChat;
interface ICChatClassObject;
EXTERN const _ELASTOS InterfaceID EIID_ICChatClassObject;
EXTERN const _ELASTOS ClassID ECLSID_CChat;
EXTERN const _ELASTOS ClassID ECLSID_CChatClassObject;





#ifdef __CHAT_USER_TYPE_H__
#include "Chat_user_type.h"
#endif

#endif // __CAR_CHAT_H__
