#include "include/configs/outbounds/amneziawg.h"

#include <QJsonArray>
#include <QUrlQuery>
#include <include/global/Utils.hpp>

#include "include/configs/common/utils.h"

namespace Configs {
    bool AmneziaPeer::ParseFromLink(const QString& link)
    {
        auto url = QUrl(link);
        if (!url.isValid()) return false;
        auto query = QUrlQuery(url.query());

        address = url.host();
        port = url.port();
        if (query.hasQueryItem("public_key")) public_key = query.queryItemValue("public_key");
        if (query.hasQueryItem("peer_public_key")) public_key = query.queryItemValue("peer_public_key");
        if (query.hasQueryItem("pre_shared_key")) pre_shared_key = query.queryItemValue("pre_shared_key");
        if (query.hasQueryItem("reserved")) {
            QString rawReserved = query.queryItemValue("reserved");
            if (!rawReserved.isEmpty()) {
                for (const auto& item : rawReserved.split("-")) {
                    int val = item.toInt();
                    if (val > 0) reserved.append(val);
                }
            }
        }
        if (query.hasQueryItem("persistent_keepalive_interval")) persistent_keepalive = query.queryItemValue("persistent_keepalive").toInt();
        
        return true;
    }

    bool AmneziaPeer::ParseFromJson(const QJsonObject& object)
    {
        if (object.isEmpty()) return false;
        if (object.contains("address")) address = object["address"].toString();
        if (object.contains("port")) port = object["port"].toInt();
        if (object.contains("public_key")) public_key = object["public_key"].toString();
        if (object.contains("pre_shared_key")) pre_shared_key = object["pre_shared_key"].toString();
        if (object.contains("reserved")) {
            reserved = QJsonArray2QListInt(object["reserved"].toArray());
        }
        if (object.contains("persistent_keepalive_interval")) persistent_keepalive = object["persistent_keepalive"].toInt();
        return true;
    }

    QString AmneziaPeer::ExportToLink()
    {
        QUrlQuery query;
        if (!public_key.isEmpty()) query.addQueryItem("public_key", public_key);
        if (!pre_shared_key.isEmpty()) query.addQueryItem("pre_shared_key", pre_shared_key);
        if (!reserved.isEmpty()) {
            QStringList reservedStr;
            for (auto val : reserved) {
                reservedStr.append(QString::number(val));
            }
            query.addQueryItem("reserved", reservedStr.join("-"));
        }
        if (persistent_keepalive > 0) query.addQueryItem("persistent_keepalive_interval", QString::number(persistent_keepalive));
        return query.toString();
    }

    QJsonObject AmneziaPeer::ExportToJson()
    {
        QJsonObject object;
        if (!address.isEmpty()) object["address"] = address;
        if (port > 0) object["port"] = port;
        if (!public_key.isEmpty()) object["public_key"] = public_key;
        if (!pre_shared_key.isEmpty()) object["pre_shared_key"] = pre_shared_key;
        if (!reserved.isEmpty()) object["reserved"] = QListInt2QJsonArray(reserved);
        if (persistent_keepalive > 0) object["persistent_keepalive_interval"] = persistent_keepalive;
        return object;
    }

    BuildResult AmneziaPeer::Build()
    {
        QJsonObject object;
        if (!address.isEmpty()) object["address"] = address;
        if (port > 0) object["port"] = port;
        if (!public_key.isEmpty()) object["public_key"] = public_key;
        if (!pre_shared_key.isEmpty()) object["pre_shared_key"] = pre_shared_key;
        if (!reserved.isEmpty()) object["reserved"] = QListInt2QJsonArray(reserved);
        if (persistent_keepalive > 0) object["persistent_keepalive_interval"] = persistent_keepalive;
        object["allowed_ips"] = QListStr2QJsonArray({"0.0.0.0/0", "::/0"});
        return {object, ""};
    }

    bool amneziawg::ParseFromLink(const QString& link)
    {
        // Standard awg:// URL format
        auto url = QUrl(link);
        if (!url.isValid()) return false;
        auto query = QUrlQuery(url.query());

        outbound::ParseFromLink(link);

        if (query.hasQueryItem("private_key")) private_key = query.queryItemValue("private_key");
        peer->ParseFromLink(link);
        
        QString rawLocalAddr = query.queryItemValue("local_address");
        if (!rawLocalAddr.isEmpty()) {
            address = rawLocalAddr.split("-");
        }
        
        if (query.hasQueryItem("mtu")) mtu = query.queryItemValue("mtu").toInt();
        if (query.hasQueryItem("use_system_interface")) system = query.queryItemValue("use_system_interface") == "true";
        if (query.hasQueryItem("workers")) worker_count = query.queryItemValue("workers").toInt();
        if (query.hasQueryItem("udp_timeout")) udp_timeout = query.queryItemValue("udp_timeout");
        
        // Amnezia WG specific parameters
        if (query.hasQueryItem("jc")) jc = query.queryItemValue("jc").toInt();
        if (query.hasQueryItem("jmin")) jmin = query.queryItemValue("jmin").toInt();
        if (query.hasQueryItem("jmax")) jmax = query.queryItemValue("jmax").toInt();
        if (query.hasQueryItem("s1")) s1 = query.queryItemValue("s1").toInt();
        if (query.hasQueryItem("s2")) s2 = query.queryItemValue("s2").toInt();
        if (query.hasQueryItem("s3")) s3 = query.queryItemValue("s3").toInt();
        if (query.hasQueryItem("s4")) s4 = query.queryItemValue("s4").toInt();
        if (query.hasQueryItem("h1")) h1 = query.queryItemValue("h1");
        if (query.hasQueryItem("h2")) h2 = query.queryItemValue("h2");
        if (query.hasQueryItem("h3")) h3 = query.queryItemValue("h3");
        if (query.hasQueryItem("h4")) h4 = query.queryItemValue("h4");
        if (query.hasQueryItem("i1")) i1 = query.queryItemValue("i1");
        if (query.hasQueryItem("i2")) i2 = query.queryItemValue("i2");
        if (query.hasQueryItem("i3")) i3 = query.queryItemValue("i3");
        if (query.hasQueryItem("i4")) i4 = query.queryItemValue("i4");
        if (query.hasQueryItem("i5")) i5 = query.queryItemValue("i5");

        return !(private_key.isEmpty() || peer->public_key.isEmpty() || server.isEmpty());
    }

    bool amneziawg::ParseFromJson(const QJsonObject& object)
    {
        if (object.isEmpty() || object["type"].toString() != "amneziawg") return false;
        outbound::ParseFromJson(object);
        if (object.contains("private_key")) private_key = object["private_key"].toString();
        if (object.contains("peers") && object["peers"].isArray() && !object["peers"].toArray().empty()) peer->ParseFromJson(object["peers"].toArray()[0].toObject());
        if (object.contains("address")) address = QJsonArray2QListString(object["address"].toArray());
        if (object.contains("mtu")) mtu = object["mtu"].toInt();
        if (object.contains("system")) system = object["system"].toBool();
        if (object.contains("worker_count")) worker_count = object["worker_count"].toInt();
        if (object.contains("udp_timeout")) udp_timeout = object["udp_timeout"].toString();
        
        // Amnezia WG specific parameters
        if (object.contains("jc")) jc = object["jc"].toInt();
        if (object.contains("jmin")) jmin = object["jmin"].toInt();
        if (object.contains("jmax")) jmax = object["jmax"].toInt();
        if (object.contains("s1")) s1 = object["s1"].toInt();
        if (object.contains("s2")) s2 = object["s2"].toInt();
        if (object.contains("s3")) s3 = object["s3"].toInt();
        if (object.contains("s4")) s4 = object["s4"].toInt();
        if (object.contains("h1")) h1 = object["h1"].toString();
        if (object.contains("h2")) h2 = object["h2"].toString();
        if (object.contains("h3")) h3 = object["h3"].toString();
        if (object.contains("h4")) h4 = object["h4"].toString();
        if (object.contains("i1")) i1 = object["i1"].toString();
        if (object.contains("i2")) i2 = object["i2"].toString();
        if (object.contains("i3")) i3 = object["i3"].toString();
        if (object.contains("i4")) i4 = object["i4"].toString();
        if (object.contains("i5")) i5 = object["i5"].toString();
        
        return true;
    }

    QString amneziawg::ExportToLink()
    {
        QUrl url;
        QUrlQuery query;
        url.setScheme("awg");
        url.setHost(peer->address);
        url.setPort(peer->port);
        if (!name.isEmpty()) url.setFragment(name);

        if (!private_key.isEmpty()) query.addQueryItem("private_key", private_key);
        
        if (!address.isEmpty()) query.addQueryItem("local_address", address.join("-"));
        if (mtu > 0 && mtu != 1408) query.addQueryItem("mtu", QString::number(mtu));
        if (system) query.addQueryItem("use_system_interface", "true");
        if (worker_count > 0) query.addQueryItem("workers", QString::number(worker_count));
        if (!udp_timeout.isEmpty()) query.addQueryItem("udp_timeout", udp_timeout);
        
        // Amnezia WG specific parameters
        if (jc > 0) query.addQueryItem("jc", QString::number(jc));
        if (jmin > 0) query.addQueryItem("jmin", QString::number(jmin));
        if (jmax > 0) query.addQueryItem("jmax", QString::number(jmax));
        if (s1 > 0) query.addQueryItem("s1", QString::number(s1));
        if (s2 > 0) query.addQueryItem("s2", QString::number(s2));
        if (s3 > 0) query.addQueryItem("s3", QString::number(s3));
        if (s4 > 0) query.addQueryItem("s4", QString::number(s4));
        if (!h1.isEmpty()) query.addQueryItem("h1", h1);
        if (!h2.isEmpty()) query.addQueryItem("h2", h2);
        if (!h3.isEmpty()) query.addQueryItem("h3", h3);
        if (!h4.isEmpty()) query.addQueryItem("h4", h4);
        if (!i1.isEmpty()) query.addQueryItem("i1", i1);
        if (!i2.isEmpty()) query.addQueryItem("i2", i2);
        if (!i3.isEmpty()) query.addQueryItem("i3", i3);
        if (!i4.isEmpty()) query.addQueryItem("i4", i4);
        if (!i5.isEmpty()) query.addQueryItem("i5", i5);
        
        mergeUrlQuery(query, outbound::ExportToLink());
        mergeUrlQuery(query, peer->ExportToLink());
        
        if (!query.isEmpty()) url.setQuery(query);
        return url.toString(QUrl::FullyEncoded);
    }

    QJsonObject amneziawg::ExportToJson()
    {
        QJsonObject object;
        object["type"] = "amneziawg";
        if (!name.isEmpty()) object["tag"] = name;
        mergeJsonObjects(object, dialFields->ExportToJson());
        if (!private_key.isEmpty()) object["private_key"] = private_key;
        if (!address.isEmpty()) object["address"] = QListStr2QJsonArray(address);
        if (mtu > 0) object["mtu"] = mtu;
        if (system) object["system"] = system;
        if (worker_count > 0) object["worker_count"] = worker_count;
        if (!udp_timeout.isEmpty()) object["udp_timeout"] = udp_timeout;
        
        // Amnezia WG specific parameters
        if (jc > 0) object["jc"] = jc;
        if (jmin > 0) object["jmin"] = jmin;
        if (jmax > 0) object["jmax"] = jmax;
        if (s1 > 0) object["s1"] = s1;
        if (s2 > 0) object["s2"] = s2;
        if (s3 > 0) object["s3"] = s3;
        if (s4 > 0) object["s4"] = s4;
        if (!h1.isEmpty()) object["h1"] = h1;
        if (!h2.isEmpty()) object["h2"] = h2;
        if (!h3.isEmpty()) object["h3"] = h3;
        if (!h4.isEmpty()) object["h4"] = h4;
        if (!i1.isEmpty()) object["i1"] = i1;
        if (!i2.isEmpty()) object["i2"] = i2;
        if (!i3.isEmpty()) object["i3"] = i3;
        if (!i4.isEmpty()) object["i4"] = i4;
        if (!i5.isEmpty()) object["i5"] = i5;
        
        auto peerObj = peer->ExportToJson();
        if (!peerObj.isEmpty()) {
            object["peers"] = QJsonArray({peerObj});
        }
        
        return object;
    }

    BuildResult amneziawg::Build()
    {
        QJsonObject object;
        object["type"] = "amneziawg";
        if (!name.isEmpty()) object["tag"] = name;
        mergeJsonObjects(object, dialFields->Build().object);
        if (!private_key.isEmpty()) object["private_key"] = private_key;
        if (!address.isEmpty()) object["address"] = QListStr2QJsonArray(address);
        if (mtu > 0) object["mtu"] = mtu;
        if (system) object["system"] = system;
        if (worker_count > 0) object["worker_count"] = worker_count;
        if (!udp_timeout.isEmpty()) object["udp_timeout"] = udp_timeout;

        // Amnezia WG specific parameters
        if (jc > 0) object["jc"] = jc;
        if (jmin > 0) object["jmin"] = jmin;
        if (jmax > 0) object["jmax"] = jmax;
        if (s1 > 0) object["s1"] = s1;
        if (s2 > 0) object["s2"] = s2;
        if (s3 > 0) object["s3"] = s3;
        if (s4 > 0) object["s4"] = s4;
        if (!h1.isEmpty()) object["h1"] = h1;
        if (!h2.isEmpty()) object["h2"] = h2;
        if (!h3.isEmpty()) object["h3"] = h3;
        if (!h4.isEmpty()) object["h4"] = h4;
        if (!i1.isEmpty()) object["i1"] = i1;
        if (!i2.isEmpty()) object["i2"] = i2;
        if (!i3.isEmpty()) object["i3"] = i3;
        if (!i4.isEmpty()) object["i4"] = i4;
        if (!i5.isEmpty()) object["i5"] = i5;

        auto peerObj = peer->Build().object;
        if (!peerObj.isEmpty()) {
            object["peers"] = QJsonArray({peerObj});
        }
        return {object, ""};
    }

    void amneziawg::SetAddress(QString newAddr) {
        peer->address = newAddr;
    }

    QString amneziawg::GetAddress() {
        return peer->address;
    }

    void amneziawg::SetPort(int newPort) {
        peer->port = newPort;
    }

    QString amneziawg::GetPort() {
        return QString::number(peer->port);
    }

    QString amneziawg::DisplayAddress()
    {
        return ::DisplayAddress(peer->address, peer->port);
    }

    QString amneziawg::DisplayType()
    {
        return "AmneziaWG";
    }

    bool amneziawg::IsEndpoint()
    {
        return true;
    }
}

