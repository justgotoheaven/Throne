#pragma once
#include "include/configs/common/Outbound.h"

namespace Configs
{
    class AmneziaPeer : public baseConfig
    {
        public:
        QString address;
        int port = 0;
        QString public_key;
        QString pre_shared_key;
        QList<int> reserved;
        int persistent_keepalive = 0;

        // baseConfig overrides
        bool ParseFromLink(const QString& link) override;
        bool ParseFromJson(const QJsonObject& object) override;
        QString ExportToLink() override;
        QJsonObject ExportToJson() override;
        BuildResult Build() override;
    };

    class amneziawg : public outbound
    {
        public:
        QString private_key;
        std::shared_ptr<AmneziaPeer> peer = std::make_shared<AmneziaPeer>();
        QStringList address;
        int mtu = 1408;
        bool system = false;
        int worker_count = 0;
        QString udp_timeout;
        
        // Amnezia WG specific parameters
        int jc = 0;           // Junk packet count
        int jmin = 0;         // Junk packet minimum size
        int jmax = 0;         // Junk packet maximum size
        int s1 = 0;           // Init packet junk size 1
        int s2 = 0;           // Init packet junk size 2
        int s3 = 0;           // Init packet junk size 3
        int s4 = 0;           // Init packet junk size 4
        QString h1;           // Init packet junk header 1
        QString h2;           // Init packet junk header 2
        QString h3;           // Init packet junk header 3
        QString h4;           // Init packet junk header 4
        QString i1;           // Init packet junk content 1
        QString i2;           // Init packet junk content 2
        QString i3;           // Init packet junk content 3
        QString i4;           // Init packet junk content 4
        QString i5;           // Init packet junk content 5

        // baseConfig overrides
        bool ParseFromLink(const QString& link) override;
        bool ParseFromJson(const QJsonObject& object) override;
        QString ExportToLink() override;
        QJsonObject ExportToJson() override;
        BuildResult Build() override;

        void SetPort(int newPort) override;
        QString GetPort() override;
        void SetAddress(QString newAddr) override;
        QString GetAddress() override;
        QString DisplayAddress() override;
        QString DisplayType() override;
        bool IsEndpoint() override;
    };
}


