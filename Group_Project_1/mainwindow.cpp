#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windowholder.h"
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QMouseEvent>

/*! \fn MainWindow::MainWindow
 * \param parent */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*! \brief labels
     * This sets up the Distance table in the user window*/
    QStringList labels;
    labels << "Destination" << "Distance (miles)";
    ui->tableWidget2->setHorizontalHeaderLabels(labels);
    ui->tableWidget2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableWidget2->verticalHeader()->hide();
    ui->tableWidget2->setShowGrid(false);
    ui->tableWidget2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget2->setSortingEnabled(false);

    /*! \brief labels2
     * This sets up the souvenir table in the user window*/
    QStringList labels2;
    labels2 << "School" << "Souvenir" << "Price";
    ui->souvenirTable->setHorizontalHeaderLabels(labels2);
    ui->souvenirTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->souvenirTable->verticalHeader()->hide();
    ui->souvenirTable->setShowGrid(false);
    ui->souvenirTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /*! \brief DB
     * This populates the drop down boxes for each user window*/
    Database *DB = Database::getInstance();
    this->populate_CD_Distance_Tracker_Combo_Box();
    this->populate_CD_School_Souvenirs_Combo_Box();

    ui->tabWidget->setCurrentIndex(0);

    this->setMouseTracking(true);

    ui->Us_pixelmap->setPixmap((QPixmap("..\\Images\\map")));
    ui->Us_pixelmap->setScaledContents(true);
    ui->Us_pixelmap->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );


    ui->FullertonButton->hide();
    ui->TexasButton->hide();
}

/*! \fn MainWindow::~MainWindow */
MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getNumOfSchool()
{
    QVector<QString> collegeNames;
    QSqlQuery query;

    query.prepare("SELECT distinct StartingCollege FROM College_Campus_Distances");

    query.exec();

    while(query.next())
    {
        collegeNames.push_back(query.value(0).toString());
    }

    return collegeNames.size();
}

void MainWindow::showFullertonAndTexas()
{
    ui->FullertonButton->show();
    ui->TexasButton->show();
}

/*! \fn MainWindow::on_CB_School_Souvenirs_currentIndexChanged
 * \param arg1 */
void MainWindow::on_CB_School_Souvenirs_currentIndexChanged(const QString &arg1)
{
    QVector<souvenir*> souvenirsList;
    QSqlQuery query;
    query.prepare("SELECT * FROM College_Souvenirs WHERE College_Souvenirs.College = :tempSchool");
    query.bindValue(0, ui->CB_School_Souvenirs->currentText());

    if (query.exec())
    {
        while(query.next())
        {
            souvenirsList.push_back(new souvenir(query.value(0).toString(), query.value(1).toString(), query.value(2).toString()));
        }
    }


    if(ui->CB_School_Souvenirs->currentText()!= "Select A School")
    {
        ui->souvenirTable->setRowCount(0);
        for(int i = 0; i < souvenirsList.size(); i++)
        {
            ui->souvenirTable->insertRow(i);
            ui->souvenirTable->setItem(i, 0, new QTableWidgetItem(souvenirsList[i]->school));
            ui->souvenirTable->setItem(i, 1, new QTableWidgetItem(souvenirsList[i]->souvenirName));
            ui->souvenirTable->setItem(i, 2, new QTableWidgetItem(souvenirsList[i]->price));
        }
    }
}

// This function populates the drop down box for School Souvenirs
/*! \fn MainWindow::populate_CD_School_Souvenirs_Combo_Box
 * This function populates the drop down box for School Souvenirs */
void MainWindow::populate_CD_School_Souvenirs_Combo_Box()
{
    ui->CB_School_Souvenirs->clear();
    QVector<souvenir*> souvenirsList;
    QSqlQuery query;
    query.prepare("PRAGMA foreign_keys = ON");

    if (query.exec("SELECT * FROM College_Souvenirs"))
    {
        while(query.next())
        {
            souvenirsList.push_back(new souvenir(query.value(0).toString(), query.value(1).toString(), query.value(2).toString()));
        }
    }

    QVector<QString> dropDownVector;
    dropDownVector.push_back("Select A School");
    bool found = false;
    for(int i = 0; i < souvenirsList.size(); i++)
    {
        for(int j = 0; j < dropDownVector.size(); j++)
        {
            if(souvenirsList[i]->school == dropDownVector[j])
            {
                found = true;
            }
        }
        if(!found)
        {
            dropDownVector.push_back(souvenirsList[i]->school);
        }
        found = false;
    }


    for(int i = 0; i < dropDownVector.size(); i++)
    {
        ui->CB_School_Souvenirs->addItem(dropDownVector[i]);
    }

}


/*! \fn MainWindow::populate_CD_Distance_Tracker_Combo_Box
 * This function populates the drop down box for Distance Tracker
 */
void MainWindow::populate_CD_Distance_Tracker_Combo_Box()
{
    ui->CB_Distance_Tracker->clear();
    QVector<college*> collegesList;
    QSqlQuery query;
    query.prepare("PRAGMA foreign_keys = ON");

    if (query.exec("SELECT * FROM College_Campus_Distances"))
    {
        while(query.next())
        {
            collegesList.push_back(new college(query.value(0).toString(), query.value(1).toString(), false, query.value(2).toDouble()));
        }
    }
    QVector<QString> dropDownVector;
    dropDownVector.push_back("Select A School");
    bool found = false;
    for(int i = 0; i < collegesList.size(); i++)
    {
        for(int j = 0; j < dropDownVector.size(); j++)
        {
            if(collegesList[i]->startingCollege == dropDownVector[j])
            {
                found = true;
            }
        }
        if(!found)
        {
            dropDownVector.push_back(collegesList[i]->startingCollege);
        }
        found = false;
    }


    for(int i = 0; i < dropDownVector.size(); i++)
    {
        ui->CB_Distance_Tracker->addItem(dropDownVector[i]);
    }

}

/*! \fn MainWindow::on_CB_Distance_Tracker_currentTextChanged
 * \param arg1 */
void MainWindow::on_CB_Distance_Tracker_currentTextChanged(const QString &arg1)
{
    QVector<college*> collegesList;
    QSqlQuery query;
    query.prepare("PRAGMA foreign_keys = ON");

    if(ui->CB_Distance_Tracker->currentText()!="Select A School")
    {
        QString schoolName = ui->CB_Distance_Tracker->currentText();
        query.prepare("SELECT EndingCollege, Distance FROM College_Campus_Distances WHERE StartingCollege = :schoolName ORDER BY Distance ASC");
        query.bindValue(0, schoolName);
        if (query.exec())
        {
            while(query.next())
            {
                collegesList.push_back(new college(schoolName, query.value(0).toString(), false, query.value(1).toDouble()));
            }
        }


        int numSchools = collegesList.size();/*ceil(sqrt(collegesList.size()));*/

        ui->tableWidget2->setRowCount(0);
        for(int i = 0; i < (numSchools); i++)
        {
            ui->tableWidget2->insertRow(i);
            ui->tableWidget2->setItem(i, 0, new QTableWidgetItem(collegesList[i]->endingCollege));
            ui->tableWidget2->setItem(i, 1, new QTableWidgetItem(QString::number(collegesList[i]->distance)));
        }


    }

}

/*! \fn MainWindow::on_backToLoginButton_clicked */
void MainWindow::on_backToLoginButton_clicked()
{
    windowHolder* WH = windowHolder::getInstance();
    WH->MainWindowHide();
    WH->LoginWindowShow();
}

void MainWindow::loadCollegesToVisit()
{
    if(!colleges.isEmpty())
    {
        colleges.clear();
    }
    Database *DB = Database::getInstance();
    QSqlQuery query;
    query.prepare("PRAGMA foreign_keys = ON");


    if (query.exec("SELECT * FROM College_Campus_Distances"))
    {
        while(query.next())
        {
            this->colleges.push_back(new college(query.value(0).toString(), query.value(1).toString(), false, query.value(2).toDouble()));
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    //    qDebug() << event->pos();
    //    QString temp = "(" + QString::number(event->pos().x());
    //    temp.append(", ");
    //    temp.append(QString::number(event->pos().y()) + ")");
    //    ui->testBrowser->setText(temp);
    //    ui->testBrowser->update();
}

/*!
 * \fn MainWindow::on_SaddlebackButton_clicked
 */
void MainWindow::on_SaddlebackButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: Saddleback College\n");
        this->loadCollegesToVisit();
        routeTracker("Saddleback College", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "Selected Saddleback College!";
}

/*!
 * \fn MainWindow::on_UCIButton_clicked
 */
void MainWindow::on_UCIButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: University of California, Irvine (UCI)\n");
        this->loadCollegesToVisit();
        routeTracker("University of California, Irvine (UCI)", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "Selected University of California, Irvine (UCI)!";
}

/*!
 * \fn MainWindow::on_UCLAButton_clicked
 */
void MainWindow::on_UCLAButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: University of California, Los Angeles (UCLA)\n");
        this->loadCollegesToVisit();
        routeTracker("University of California, Los Angeles (UCLA)", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "Selected University of California, Los Angeles (UCLA)!";
}

/*! \fn routeTracker
 * \param schoolName
 * \param numOfSchoolsVisiting
 * \param totalMiles
 * \param collegesToVisit */
void MainWindow::routeTracker(QString schoolName,           // Name of school that gets passed in
                  int numOfSchoolsVisiting,     // Total number of schools visiting
                  double &totalMiles,              // Total mileage of the trip
                  QVector<college*> collegesToVisit){// Vector of schools we are visiting
    /*! \brief Our database*/
    Database *DB = Database::getInstance();

    /*! \brief Closest school in comparison to schoolName*/
    college* closestSchool = DB->getClosestSchool(schoolName, collegesToVisit);

    QString closestSchoolName = closestSchool->endingCollege;

    /*! Base Case*/
    if(numOfSchoolsVisiting == 0)
    {
        return; /*! \brief This will leave the program if out number of schools to visit is 0
                    and not return anything since the function is void*/
    } else
    {
        totalMiles += closestSchool->distance;

        ui->testBrowser->append("From " + closestSchool->startingCollege + " to " +
                                closestSchool->endingCollege + " is " + QString::number(closestSchool->distance) + " miles\n");

        /*! \brief Calls the function again*/
        routeTracker(closestSchoolName, --numOfSchoolsVisiting, totalMiles, collegesToVisit);
    }
}

/*!
 * \fn MainWindow::on_UniversityOfPacificButton_clicked
 */
void MainWindow::on_UniversityOfPacificButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: University of the Pacific\n");
        this->loadCollegesToVisit();
        routeTracker("University of the Pacific", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "University of the Pacific!";
}

/*!
 * \fn MainWindow::on_UniversityOfOregonButton_clicked
 */
void MainWindow::on_UniversityOfOregonButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: University of Oregon\n");
        this->loadCollegesToVisit();
        routeTracker("University of Oregon", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "University of Oregon!";
}

/*!
 * \fn MainWindow::on_ASUButton_clicked
 */
void MainWindow::on_ASUButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: Arizona State University\n");
        this->loadCollegesToVisit();
        routeTracker("Arizona State University", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "Arizona State University!";

}

/*!
 * \fn MainWindow::on_UniversityOfWisconsinButton_clicked
 */
void MainWindow::on_UniversityOfWisconsinButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: University of Wisconsin\n");
        this->loadCollegesToVisit();
        routeTracker("University of Wisconsin", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "University of Wisconsin!";

}

/*!
 * \fn MainWindow::on_NorthwesternButton_clicked
 */
void MainWindow::on_NorthwesternButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: Northwestern\n");
        this->loadCollegesToVisit();
        routeTracker("Northwestern", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "Northwestern!";

}

/*!
 * \fn MainWindow::on_UniversityOfMichiganButton_clicked
 */
void MainWindow::on_UniversityOfMichiganButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: University of Michigan\n");
        this->loadCollegesToVisit();
        routeTracker("University of Michigan", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "University of Michigan!";

}

/*!
 * \fn MainWindow::on_MITButton_clicked
 */
void MainWindow::on_MITButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: Massachusetts Institute of Technology (MIT)\n");
        this->loadCollegesToVisit();
        routeTracker("Massachusetts Institute of Technology (MIT)", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "Massachusetts Institute of Technology (MIT)!";

}

/*!
 * \fn MainWindow::on_OSUButton_clicked
 */
void MainWindow::on_OSUButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: Ohio State University\n");
        this->loadCollegesToVisit();
        routeTracker("Ohio State University", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "Ohio State University!";

}

/*!
 * \fn MainWindow::on_keyButton_clicked
 */
void MainWindow::on_keyButton_clicked()
{
    windowHolder *WH = windowHolder::getInstance();
    WH->keyWindowShow();
}

void MainWindow::on_FullertonButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: California State University, Fullerton");
        this->loadCollegesToVisit();
        routeTracker("California State University, Fullerton", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "California State University, Fullerton!";
}

void MainWindow::on_TexasButton_clicked()
{
    double totalMiles = 0;
    int numOfSchools = this->getNumOfSchool();
    ui->testBrowser->clear();
    ui->TotalMilesLabel->clear();
    if(ui->automaticRouteTrackingCheckBox->isChecked())
    {
        ui->testBrowser->setText("Starting School: University of Texas");
        this->loadCollegesToVisit();
        routeTracker("University of Texas", numOfSchools - 1, totalMiles, colleges);
        QTextCursor cursor = ui->testBrowser->textCursor();
        cursor.setPosition(0);
        ui->testBrowser->setTextCursor(cursor);
        ui->TotalMilesLabel->setText("Total Miles: " + QString::number(totalMiles));
    }
    qDebug() << "University of Texas!";

}
