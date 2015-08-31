#if !defined(_IZANAGI_NETWORK_IP_V4_ENDPOINT_H__)
#define _IZANAGI_NETWORK_IP_V4_ENDPOINT_H__

#include "izDefs.h"
#include "network/NetworkDefs.h"

namespace izanagi {
namespace net {
    /** IPv4�A�h���X.
     */
    class IPv4Address {
        friend class IPv4Endpoint;

    public:
        /** �T�[�o�[�����ׂẴl�b�g���[�N�C���^�[�t�F�C�X�ŃN���C�A���g�ɂ��l�b�g���[�N���p��ҋ@����K�v�����邱�Ƃ������A�h���X.
         */
        static IPv4Address Any;

        IPv4Address();

        /** �R���X�g���N�^.
         * IPv4�A�h���X�� [a.b.c.d] �ƕ������Đݒ�.
         */
        IPv4Address(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d);

        /** �R���X�g���N�^.
         * IPv4�A�h���X��32bit�����l�Őݒ�.
         */
        IPv4Address(IZ_UINT32 ip);

        /** �R���X�g���N�^.
         * �z�X�g������IPv4�A�h���X�������Őݒ�.
         */
        IPv4Address(const IZ_CHAR* host);

        ~IPv4Address();

        IPv4Address(const IPv4Address& rhs);

        const IPv4Address& operator=(const IPv4Address& rhs);

    public:
        /** IPv4�� [a.b.c.d] �ƕ������Đݒ�.
         */
        void set(IZ_UINT8 a, IZ_UINT8 b, IZ_UINT8 c, IZ_UINT8 d);

        /** IPv4�A�h���X��32bit�����l�Őݒ�.
         */
        void set(IZ_UINT32 ip);

        /** IPv4�A�h���X�𕶎���i"WW.XX.YY.ZZ"�j�Őݒ�.
         */
        void set(const IZ_CHAR* ip);

        /** �w�肳�ꂽ�z�X�g������IPv4�A�h���X�������Ŏ擾.
         */
        IZ_BOOL setByHostName(const IZ_CHAR* host);

        /** IPv4�A�h���X��32bit�����l�Ƃ��Ď擾.
         * @return IPv4�A�h���X��32bit�����l�ł���킵�����̂�Ԃ�.
         */
        IZ_UINT32 getValue() const;

        /** IPv4�A�h���X�𕶎���Ƃ��Ď擾.
         * @param [out] ret ������i�[��̃o�b�t�@�������|�C���^.
         * @param [in] size ������i�[��̃o�b�t�@�̃T�C�Y�ibytes�j.
         */
        void toString(IZ_CHAR* ret, IZ_UINT size) const;

        /** �w�肳�ꂽ IPv4Address �Ɣ�r.
         * @param [in] rhs ��r������ IPv4Address.
         * @return ��v����ꍇ�� true�A��v���Ȃ��ꍇ�� false ��Ԃ�.
         */
        IZ_BOOL operator==(const IPv4Address& rhs);

        /** �w�肳�ꂽ IPv4Address �Ɣ�r.
         * @param [in] rhs ��r������ IPv4Address.
         * @return ��v���Ȃ��ꍇ�� true�A��v����ꍇ�� false ��Ԃ�.
         */
        IZ_BOOL operator!=(const IPv4Address& rhs);

        /** ���ׂẴl�b�g���[�N�C���^�[�t�F�C�X�ŃN���C�A���g�ɂ��l�b�g���[�N���p��ҋ@����K�v�����邱�Ƃ������A�h���X���ǂ������擾.
         * @return �ҋ@����K�v������A�h���X�Ȃ� true�A�����łȂ���� false ��Ԃ�.
         */
        IZ_BOOL isAny() const;

    private:
        IZ_BOOL setByHostName();

    private:
        union {
            IZ_UINT8 p[4];
            IZ_UINT32 value;
        } m_ip;
    };

    /** IPv4�A�h���X�ƃ|�[�g�ԍ��Ńl�b�g���[�N�G���h�|�C���g��\��.
     */
    class IPv4Endpoint {
    public:
        /** �R���X�g���N�^
         * IPv4�A�h���X��Any�A�|�[�g�ԍ���0�ŏ�����.
         */
        IPv4Endpoint();

        /** �R���X�g���N�^.
         * �w�肳�ꂽIPv4�A�h���X�ƃ|�[�g�ԍ��ŏ�����.
         */
        IPv4Endpoint(
            const IPv4Address& address,
            IZ_UINT port);

        ~IPv4Endpoint();

        IPv4Endpoint(const IPv4Endpoint& rhs);

        const IPv4Endpoint& operator=(const IPv4Endpoint& rhs);

    public:
        /** IPv4�A�h���X�ƃ|�[�g�ԍ���ݒ�.
         * @param [in] address IPv4�A�h���X.
         * @paran [in] port �|�[�g�ԍ�.
         */
        void set(
            const IPv4Address& address,
            IZ_UINT port);

        /** �ݒ肳��Ă���IPv4�A�h���X���擾.
         * @return �ݒ肳��Ă���IPv4�A�h���X��Ԃ�.
         */
        const IPv4Address& getAddress() const;

        /** �ݒ肳��Ă���|�[�g�ԍ����擾.
         * @return �ݒ肳��Ă���|�[�g�ԍ���Ԃ�.
         */
        IZ_UINT getPort() const;

        /** �w�肳�ꂽ IPv4Endpoint �Ɣ�r.
         * @param [in] rhs ��r������ IPv4Endpoint.
         * @return ��v����ꍇ�� true�A��v���Ȃ��ꍇ�� false ��Ԃ�.
         */
        IZ_BOOL operator==(const IPv4Endpoint& rhs);

        /** �w�肳�ꂽ IPv4Endpoint �Ɣ�r.
         * @param [in] rhs ��r������ IPv4Address.
         * @return ��v���Ȃ��ꍇ�� true�A��v����ꍇ�� false ��Ԃ�.
         */
        IZ_BOOL operator!=(const IPv4Endpoint& rhs);

        void set(const sockaddr_in& addr);
        void get(sockaddr_in& addr);
        IZ_BOOL operator==(const sockaddr_in& addr);

    private:
        IPv4Address m_address;
        IZ_UINT m_port;
    };
}    // namespace net
}    // namespace izanagi

#endif    // #if !defined(_IZANAGI_NETWORK_IP_V4_ENDPOINT_H__)
