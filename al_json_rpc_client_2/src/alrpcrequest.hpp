
#ifndef ALRPCREQUEST_H
#define ALRPCREQUEST_H

class AlRpcRequest {
public:
  enum SERVER_MESSAGE_TYPE {
    NONE = 1,
    AUTHENTICATE, // authenticate [ token ] -> boolean аутентификация
                  // соединения;
    USER_MODE,
    ROOM_OPEN, // room/open [ name, p2p ] -> boolean создание комнаты;
    ROOM_CLOSE, // room/close -> boolean закрытие текущей комнат;
    ROOM_ENTER, // room/enter [ name ] -> boolean вход в созданную комнату;
    ROOM_LIVE, // room/leave -> boolean выход из текущей комнаты;
    ROOM_OFFER, // room/offer [ offerSdp ] -> answerSdp отправление SDP
                // предложения и получение SDP отклика в результате.
    ROOM_ICE_CANDIDATE, // room/ice-candidate [ candidate ] передача
                        // обнаруженного ICE candidate.
    OFFER,              // offer [ offerSdp ] -> answerSdp отправление SDP предложения и
    // получение SDP отклика в результате.
    ICE_CANDIDATE, // ice-candidate [ candidate ] передача обрнаруженного ICE
    // candidate.
  };

  AlRpcRequest();
  AlRpcRequest(long id, int type);

  long m_id;
  int m_type;
};

#endif // ALRPCREQUEST_H
