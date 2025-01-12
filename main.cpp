#include "mainwindow.h"

#include <QApplication>
#include "api_client/IpStackApiClient.h"
#include "api_client/IpResolver.h"
#include "db/SqlLiteDataStorage.h"

int main(int argc, char *argv[])
{   
    qRegisterMetaType<models::ItemListModel>();

    auto client = std::make_shared<api_client::IPStackApiClient>();
    auto db = std::make_shared<SqlLiteDataStorage>();
    auto ipResolver = std::make_shared<api_client::IPResolver>();

    QApplication a(argc, argv);
    MainWindow w;
    w.InjectGeolocationProvider(client);
    w.InjectAndSetupDB(db);
    w.InjectIpResolver(ipResolver);
    w.show();
    return a.exec();
}
