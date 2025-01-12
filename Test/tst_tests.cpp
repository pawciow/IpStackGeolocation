#include <QtTest>

#include "./ui_mainwindow.h"
#include "../mainwindow.h"

#include <QPushButton>
#include <QSignalSpy>
#include <QMessageBox>

#include <memory>
#include <QMainWindow>

#include "mocks/MockGeolocationProvider.h"
#include "mocks/MockDataStorage.h"
#include "mocks/MockIPResolver.h"

using ::testing::_;
using ::testing::Return;
using ::testing::Throw;

std::string googleHostname = "google.com";
std::string googleIPV4 = "142.250.189.238";
models::Geolocation googleGeolocation = {40.7128, -74.0060};

class Tests : public QObject
{
    Q_OBJECT
    friend class MainWindow;

public:
    Tests();
    ~Tests();

private:
    std::unique_ptr<MainWindow> _window;
    std::shared_ptr<api_client::MockGeolocationProvider> _mockGeolocationProvider;
    std::shared_ptr<api_client::MockIpResolver> _mockIpResolver;
    std::shared_ptr<MockDataStorage> _mockDataStorage;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void initWindow();
    void resetWindow();
    void resetWindowState();

    void test_AddButtonClicks_EmptyInput();
    void test_AddButtonClicks_InvalidInput();
    void test_AddButtonClicks_CorrectIPInput();
    void test_AddButtonClicks_CorrectHostnameInput();
    void test_AddButtonClicks_AddDuplicate();

    void test_viewOnMapButtonClicks_NoSelection();
    void test_viewOnMapButtonClicks_MoveToSelected();

    void test_RemoveButtonClicks_NoSelection();
    void test_RemoveButtonClicks_RemovedSelected();
};

Tests::Tests() {}

Tests::~Tests() {}

void Tests::initTestCase()
{
    initWindow();
}

void Tests::cleanupTestCase()
{
    resetWindow();
}

void Tests::initWindow()
{
    _window = std::make_unique<MainWindow>();
    _mockGeolocationProvider = std::make_shared<api_client::MockGeolocationProvider>();
    _mockIpResolver = std::make_shared<api_client::MockIpResolver>();
    _mockDataStorage = std::make_shared<MockDataStorage>();

    _window->InjectGeolocationProvider(_mockGeolocationProvider);

    EXPECT_CALL(*_mockDataStorage, Connect())
        .Times(1);
    EXPECT_CALL(*_mockDataStorage, FetchAll())
        .Times(1);
    _window->InjectAndSetupDB(_mockDataStorage);
    _window->InjectIpResolver(_mockIpResolver);
}

void Tests::resetWindow()
{
    _window.reset();
    _mockGeolocationProvider.reset();
    _mockIpResolver.reset();
    _mockDataStorage.reset();
}

void Tests::resetWindowState()
{
    resetWindow();
    initWindow();
}

void Tests::test_AddButtonClicks_EmptyInput()
{
    QSignalSpy addLocationSpy(&(*_window), &MainWindow::addLocationMarking);

    QPushButton* addButton = _window->findChild<QPushButton*>("pushButton");
    QVERIFY(addButton != nullptr);

    QTest::mouseClick(addButton, Qt::LeftButton);
    QCOMPARE(addLocationSpy.count(), 0);
}

void Tests::test_AddButtonClicks_InvalidInput()
{
    resetWindowState();

    QSignalSpy addLocationSpy(&(*_window), &MainWindow::addLocationMarking);
    QPushButton* addButton = _window->findChild<QPushButton*>("pushButton");
    QVERIFY(addButton != nullptr);

    QString ipAddress = "INVALID_INPUT";
    _window->ui->inputTextEdit->setPlainText(ipAddress);

    EXPECT_CALL(*_mockIpResolver, IsValidIP(_))
        .WillOnce(Return(false));
    EXPECT_CALL(*_mockIpResolver, FetchIpFromHostname(_))
        .WillOnce(Throw(std::runtime_error("Failed to fetch IP from hostname")));

    QTest::mouseClick(addButton, Qt::LeftButton);
    QCOMPARE(addLocationSpy.count(), 0);
}

void Tests::test_AddButtonClicks_CorrectIPInput()
{
    resetWindowState();

    QSignalSpy addLocationSpy(&(*_window), &MainWindow::addLocationMarking);
    QPushButton* addButton = _window->findChild<QPushButton*>("pushButton");
    QVERIFY(addButton != nullptr);

    QString ipAddress = QString::fromStdString(googleIPV4);
    _window->ui->inputTextEdit->setPlainText(ipAddress);

    EXPECT_CALL(*_mockIpResolver, IsValidIP(_))
        .WillOnce(Return(true));

    EXPECT_CALL(*_mockGeolocationProvider, GetGeolocation(googleIPV4))
        .WillOnce(Return(googleGeolocation));

    EXPECT_CALL(*_mockDataStorage, Store(_))
        .Times(1);

    QTest::mouseClick(addButton, Qt::LeftButton);
    QCOMPARE(addLocationSpy.count(), 1);
}

void Tests::test_AddButtonClicks_CorrectHostnameInput()
{
    resetWindowState();

    QSignalSpy addLocationSpy(&(*_window), &MainWindow::addLocationMarking);
    QPushButton* addButton = _window->findChild<QPushButton*>("pushButton");
    QVERIFY(addButton != nullptr);

    QString ipAddress = QString::fromStdString(googleHostname);
    _window->ui->inputTextEdit->setPlainText(ipAddress);

    EXPECT_CALL(*_mockIpResolver, IsValidIP(_))
        .WillOnce(Return(false));

    EXPECT_CALL(*_mockIpResolver, FetchIpFromHostname(_))
        .WillOnce(Return(googleIPV4));

    EXPECT_CALL(*_mockGeolocationProvider, GetGeolocation(googleIPV4))
        .WillOnce(Return(googleGeolocation));

    EXPECT_CALL(*_mockDataStorage, Store(_))
        .Times(1);

    QTest::mouseClick(addButton, Qt::LeftButton);
    QCOMPARE(addLocationSpy.count(), 1);
}

void Tests::test_AddButtonClicks_AddDuplicate()
{
    resetWindowState();
    auto startCount = _window->ui->listWidget->count();
    std::cout << startCount;

    QSignalSpy addLocationSpy(&(*_window), &MainWindow::addLocationMarking);
    QPushButton* addButton = _window->findChild<QPushButton*>("pushButton");
    QVERIFY(addButton != nullptr);

    QString ipAddress = QString::fromStdString(googleIPV4);
    _window->ui->inputTextEdit->setPlainText(ipAddress);

    EXPECT_CALL(*_mockIpResolver, IsValidIP(_))
        .WillOnce(Return(true));

    EXPECT_CALL(*_mockGeolocationProvider, GetGeolocation(googleIPV4))
        .WillOnce(Return(googleGeolocation));

    EXPECT_CALL(*_mockDataStorage, Store(_))
        .Times(1);

    QTest::mouseClick(addButton, Qt::LeftButton);
    QCOMPARE(addLocationSpy.count(), 1);

    EXPECT_CALL(*_mockIpResolver, IsValidIP(_))
        .WillOnce(Return(true));

    EXPECT_CALL(*_mockGeolocationProvider, GetGeolocation(googleIPV4))
        .WillOnce(Return(googleGeolocation));

    QTest::mouseClick(addButton, Qt::LeftButton);
    QCOMPARE(addLocationSpy.count(), 1);
}

void Tests::test_viewOnMapButtonClicks_NoSelection()
{
    resetWindowState();

    QSignalSpy changePositionSpy(&(*_window), &MainWindow::changeMapPosition);

    QPushButton* viewOnMapButton = _window->findChild<QPushButton*>("viewOnMapButton");
    QVERIFY(viewOnMapButton != nullptr);

    QTest::mouseClick(viewOnMapButton, Qt::LeftButton);
    QCOMPARE(changePositionSpy.count(), 0);
}

void Tests::test_viewOnMapButtonClicks_MoveToSelected()
{
    resetWindowState();

    auto startCount = _window->ui->listWidget->count();
    models::ItemListModel item;
    item.ip = googleIPV4;
    item.geolocation = googleGeolocation;

    QListWidgetItem* listItem = new QListWidgetItem(QString::fromStdString(item.ip));
    listItem->setData(Qt::UserRole, QVariant::fromValue(item));

    _window->ui->listWidget->addItem(listItem);
    QCOMPARE(_window->ui->listWidget->count(), startCount+1);

    int row = _window->ui->listWidget->row(listItem);
    _window->ui->listWidget->selectionModel()->select(
        _window->ui->listWidget->model()->index(row, 0),
        QItemSelectionModel::Select
    );

    QSignalSpy changePositionSpy(&(*_window), &MainWindow::changeMapPosition);

    QPushButton* viewOnMapButton = _window->findChild<QPushButton*>("viewOnMapButton");
    QVERIFY(viewOnMapButton != nullptr);

    QTest::mouseClick(viewOnMapButton, Qt::LeftButton);
    QCOMPARE(changePositionSpy.count(), 1);

}

void Tests::test_RemoveButtonClicks_NoSelection()
{
    resetWindowState();

    QSignalSpy removeLocationSpy(&(*_window), &MainWindow::removeLocationMarking);

    QPushButton* removeButton = _window->findChild<QPushButton*>("removeButton");
    QVERIFY(removeButton != nullptr);

    QTest::mouseClick(removeButton, Qt::LeftButton);
    QCOMPARE(removeLocationSpy.count(), 0);
}

void Tests::test_RemoveButtonClicks_RemovedSelected()
{
    resetWindowState();

    auto startCount = _window->ui->listWidget->count();
    models::ItemListModel item;
    item.ip = googleIPV4;
    item.geolocation = googleGeolocation;

    QListWidgetItem* listItem = new QListWidgetItem(QString::fromStdString(item.ip));
    listItem->setData(Qt::UserRole, QVariant::fromValue(item));

    _window->ui->listWidget->addItem(listItem);
    QCOMPARE(_window->ui->listWidget->count(), startCount+1);

    int row = _window->ui->listWidget->row(listItem);
    _window->ui->listWidget->selectionModel()->select(
        _window->ui->listWidget->model()->index(row, 0),
        QItemSelectionModel::Select
    );

    QSignalSpy removeLocationSpy(&(*_window), &MainWindow::removeLocationMarking);
    QPushButton* removeButton = _window->findChild<QPushButton*>("removeButton");
    QVERIFY(removeButton != nullptr);
    EXPECT_CALL(*_mockDataStorage, Remove(_))
        .Times(1);
    QTest::mouseClick(removeButton, Qt::LeftButton);

    QCOMPARE(removeLocationSpy.count(), 1);

    QList<QVariant> arguments = removeLocationSpy.takeFirst();
    QCOMPARE(arguments.at(0).toDouble(), googleGeolocation.latitude);
    QCOMPARE(arguments.at(1).toDouble(), googleGeolocation.longitude);
    QCOMPARE(arguments.at(2).toString(), googleIPV4);

    QCOMPARE(_window->ui->listWidget->count(), startCount);
}

QTEST_MAIN(Tests)

#include "tst_tests.moc"
