#include "mainwindow.h"
#include "ui_mainwindow.h"
pthread_t thread_config;
pthread_t thread_crateonoff;

//#include "QCaLabelPlugin.h"
QCaComboBox *ggg;

//HvCrate::HvChannel HvCrate::hvch[HvCrate::CHS_NUM];
HvCrate::HvChannel HvCrate::hvch[HvCrate::CHS_NUM];
BoardButton *HvCrate::current_board;
QLabel *HvCrate::label_hvpar[PARS_NUM];
int HvCrate::constructed;
int MainWindow::constructed;
CrateButton *MainWindow::current_crate;
HvCrate *MainWindow::hvcrate;
QPushButton *MainWindow::bback;

int HvCrate::crate_ioc_type; // my_n2:
QString HvCrate::slot_label; // my_n2:
QString HvCrate::channel_label; // my_n2:

int HvCrate::params_read;
int HvCrate::params_switch;
int HvCrate::params_set;
int HvCrate::params_num;
int HvCrate::channels_num;
int HvCrate::first_channel;
int HvCrate::crate_type;
QTimer *HvCrate::timer_db;

QLabel *HvCrate::lab_cr_name;
QPushButton *HvCrate::bturnonallchannels;
QPushButton *HvCrate::bturnoffallchannels;
QPushButton *HvCrate::bcrateon;
QPushButton *HvCrate::bcrateoff;
QPushButton *HvCrate::bstore;
QPushButton *HvCrate::bload;
QLabel *HvCrate::label_chnumber_title;
QLabel *HvCrate::label_log_name;

QPushButton *HvCrate::bsetallchannels;
int HvCrate::current_channel=-1;
int HvCrate::current_parameter=-1;
//int HvCrate::chs_num;
//BoardButton *HvCrate::current_board_prev;

/** ---------------------------------------*/
char *epics_hbeat_suffix_parameter=".T"; // my_n:
int hbeat_param_number=15; // my_n;

char *epics_log_name_suffix_parameter_caen=".DESC";
char *epics_log_name_suffix_parameter_mpod="_stat.DESC";
/** ----------------------------------------*/

char *epics_suffixes_parameters_caen[]={".F",".E",".L","_pwonoff","_v0set","_i0set","_trip","_rampup","_rampdn"}; // vmon, imon, status
char *params_labels_caen[]={"vmon","imon","status","on/off","v0set","i0set","trip","rampup","rampdn"}; // vmon, imon, status

char *epics_suffixes_parameters_wmpodhv[]={"_v_term","_i_rd","_stat_string","_switch","_v_set","_i_set","_i_trip_time","_vrise","_vfall"};

/// char *epics_suffixes_parameters_caen[]={"L","pwonoff","v0set","i0set","trip","rampup","rampdn"}; // vmon, imon, status

char *statuses_1527[]={"ON","RUP","RDN","OVC","OVV","UNV","ExTrip","MAXV","ExDis","InTrip","CalEr", "ChUn"};
char *statuses_527[]= {"ON","RUP","RDN","OVC","UNV","OVV","ExTrip","MAXV","     ","Kill",  "InTrip"};

enum istatuses_1527 {ON,RUP,RDN,OVC,OVV,UNV,ExTrip,MAXV,ExDis,InTrip,CalEr, ChUn};
//char *statuses_527[]= {"ON","RUP","RDN","OVC","UNV","OVV","ExTrip","MAXV","     ","Kill",  "InTrip"};



//==================CONFIGURATION PIECE==============================================================

//------  my_n2:
#define NCRATES_CIOC 15
#define NCRATES_CIOCOLD 1
#define NCRATES_MIOC 1

#define NCRATES 17
#define NBOARDS 16
//-------
char ***board_names_by_crate_cioc=NULL; // my_n2:


 char *board_names_by_crate_ciocold[NCRATES_CIOCOLD][NBOARDS]={ /// my_n2
    {"B_DCRB_HV000_0_P0", "B_DCRB_HV000_1_P0", "B_DCRB_HV000_2_P0", "B_DCRB_HV000_3_P0", "B_DCRB_HV000_4_P0",
     "B_DCRB_HV000_5_P0", "B_DCRB_HV000_6_P0", "B_DCRB_HV000_7_P0", "B_DCRB_HV000_8_P0", "B_DCRB_HV000_9_P0",
     "","","" ,"","",""}
 };

 char *board_names_by_crate_mioc[NCRATES_MIOC][NBOARDS]={ /// my_n2
    {"B_SVTMPOD9_Sl0_P0", "B_SVTMPOD9_Sl1_P0", "B_SVTMPOD9_Sl2_P0", "B_SVTMPOD9_Sl3_P0", "B_SVTMPOD9_Sl4_P0",
     "B_SVTMPOD9_Sl5_P0", "B_SVTMPOD9_Sl6_P0", "B_SVTMPOD9_Sl7_P0", "B_SVTMPOD9_Sl8_P0", "B_SVTMPOD9_Sl9_P0",
     "","","" ,"","",""}

    //    {"b_wmpod_hv000_0_P0", "b_wmpod_hv000_1_P0", "b_wmpod_hv000_2_P0","","","","","","","","","","" ,"","",""}
  };

 char *hv_crate_names_cioc[]={"HVTEST0", "HVFTOF6", "HVFTOF5", "HVFTOF4",
                         "HVECAL1", "HVFTOF1", "HVFTOF2", "HVFTOF3",
                         "HVECAL5", "HVECAL6", "HVECAL2", "HVECAL3",
 //                       "HVECAL5",/* "HVECAL6", "HVECAL2",*/ "HVECAL3",
                         "HVLTCC0", "HVECAL4", "HVCTOF0"};

int **board_types_by_crate_cioc; // my_n2:

int board_types_by_crate_ciocold[NCRATES_CIOCOLD][NBOARDS]={ /// my_n2
    {HvCrate::CAEN_A944, HvCrate::CAEN_A944, HvCrate::CAEN_A944, HvCrate::CAEN_A944, HvCrate::CAEN_A944, HvCrate::CAEN_A944,
     HvCrate::CAEN_A944, HvCrate::CAEN_A944, HvCrate::CAEN_A944, HvCrate::CAEN_A944, 0,0,0, 0,0,0},
};

int board_types_by_crate_mioc[NCRATES_MIOC][NBOARDS]={ /// my_n2
    {HvCrate::WMPOD_LV, HvCrate::WMPOD_LV, HvCrate::WMPOD_LV, 0, 0,
     0,0,0, HvCrate::WMPOD_HV, 0,
     0,0,0, 0,0,0}
};

char *hv_crate_names[]={"HVTEST0 \n CAEN 1527", "HVFTOF6 \n CAEN 1527", "HVFTOF5 \n CAEN 1527", "HVFTOF4 \n CAEN 4527",
                        "HVECAL1 \n CAEN 4527", "HVFTOF1 \n CAEN 4527", "HVFTOF2 \n CAEN 4527", "HVFTOF3 \n CAEN 1527",
                        "HVECAL5 \n CAEN 4527", "HVECAL6 \n CAEN 4527", "HVECAL2 \n CAEN 4527", "HVECAL3 \n CAEN 4527",
//                        "HVECAL5 \n CAEN 4527",/* "HVECAL6 \n CAEN 4527", "HVECAL2 \n CAEN 4527",*/ "HVECAL3 \n CAEN 4527",
                        "HVLTCC0 \n CAEN 1527", "HVECAL4 \n CAEN 4527", "HVCTOF0 \n CAEN 1527", "HVDCRB \n CAEN 527",
                        "HVSVT \n Wiener MPOD"};

char hv_crate_types[]={CrateButton::CAEN_1527, CrateButton::CAEN_1527, CrateButton::CAEN_1527, CrateButton::CAEN_1527,
                       CrateButton::CAEN_1527, CrateButton::CAEN_1527, CrateButton::CAEN_1527, CrateButton::CAEN_1527,
                       CrateButton::CAEN_1527, CrateButton::CAEN_1527, CrateButton::CAEN_1527, CrateButton::CAEN_1527,
//                       CrateButton::CAEN_1527,/* CrateButton::CAEN_1527, CrateButton::CAEN_1527, */CrateButton::CAEN_1527,
                       CrateButton::CAEN_1527, CrateButton::CAEN_1527, CrateButton::CAEN_1527, CrateButton::CAEN_527,
                       CrateButton::WMPOD};

//==================END OF CONFIGURATION PIECE==============================================================


///----- my_n2:
char *slot_labels_in_record_name_by_ioc_type[]=
{"Sl","","Sl"};
char *channel_labels_in_record_name_by_ioc_type[]=
{"Ch","","Ch"};
///------------



//=============================================================================================== my_n2:
/*
char *fun_board_names_by_crate(int i, int j){
 if(i<NCRATES_CIOC)return board_names_by_crate_cioc[i][j];
 else if(i<(NCRATES_CIOC+NCRATES_CIOCOLD))return (char *)board_names_by_crate_ciocold[i-NCRATES_CIOC][j];
 else if(i<(NCRATES_CIOC+NCRATES_CIOCOLD+NCRATES_MIOC))return (char *)board_names_by_crate_mioc[i-NCRATES_CIOC-NCRATES_CIOCOLD][j];
}
*/
//------------
char **fun_board_names_by_crate(int i){
 if(i<NCRATES_CIOC)return board_names_by_crate_cioc[i];
 else if(i<(NCRATES_CIOC+NCRATES_CIOCOLD))return (char **)board_names_by_crate_ciocold[i-NCRATES_CIOC];
 else if(i<(NCRATES_CIOC+NCRATES_CIOCOLD+NCRATES_MIOC))return (char **)board_names_by_crate_mioc[i-NCRATES_CIOC-NCRATES_CIOCOLD];
}
//------------

int *fun_board_types_by_crate(int i){
 if(i<NCRATES_CIOC)return board_types_by_crate_cioc[i];
 else if(i<(NCRATES_CIOC+NCRATES_CIOCOLD))return (int *)board_types_by_crate_ciocold[i-NCRATES_CIOC];
 else if(i<(NCRATES_CIOC+NCRATES_CIOCOLD+NCRATES_MIOC))return (int *)board_types_by_crate_mioc[i-NCRATES_CIOC-NCRATES_CIOCOLD];
}
//------------
void HvCrate::get_crate_ioc_type(int i){
 if(i<NCRATES_CIOC)crate_ioc_type=HvCrate::CAEN_IOC;
 else if(i<(NCRATES_CIOC+NCRATES_CIOCOLD))crate_ioc_type=HvCrate::CAEN_IOCOLD;
 else if(i<(NCRATES_CIOC+NCRATES_CIOCOLD+NCRATES_MIOC))crate_ioc_type=HvCrate::MPOD_IOC;
}
//------------
void gen_names_and_types_cioc(){

    char tmp[1024];

      board_names_by_crate_cioc = (char ***)malloc((sizeof(board_names_by_crate_cioc))*(NCRATES_CIOC));

      board_types_by_crate_cioc = (int **)malloc((sizeof(board_types_by_crate_cioc))*(NCRATES_CIOC));

     for(int i=0;i<NCRATES_CIOC;i++){
       board_names_by_crate_cioc[i]= (char **)malloc((sizeof(board_names_by_crate_cioc))*NBOARDS);
       board_types_by_crate_cioc[i] = (int *)malloc((sizeof(int))*(NBOARDS));
      for(int j=0; j< NBOARDS; j++){
      //   printf("1 %ld %d %d\n",sizeof(board_names_by_crate_cioc), nn, NBOARDS); fflush(stdout);

      //sprintf(tmp, "B_%s_sl%d_P0", hv_crate_names[i],j);
   ///       sprintf(tmp, "B_HV%03d_%s%d_P0", i,slot_labels_in_record_name_by_ioc_type[HvCrate::CAEN_IOC],j);
       sprintf(tmp, "B_%s_%s%d_P0", hv_crate_names_cioc[i],slot_labels_in_record_name_by_ioc_type[HvCrate::CAEN_IOC],j);
       //printf("%s %d %d len=%d\n", tmp, i, j, strlen(tmp)); fflush(stdout);
      // printf("==%s==\n", tmp); fflush(stdout);
       board_names_by_crate_cioc[i][j]=(char *)malloc(strlen(tmp)+1);
      // printf("%s\n", tmp); fflush(stdout);
       strcpy(board_names_by_crate_cioc[i][j], tmp);
       board_types_by_crate_cioc[i][j]=HvCrate::CAEN_A1535;
 //       printf("%s\n", tmp); fflush(stdout);
      }
     }

}


//===============================================================================================
MainWindow *mainw;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
resize(907+HvCrate::TABLE_XCELLLOG,757); /// my_n:
gen_names_and_types_cioc();/// my_n2

    setenv("EPICS_CA_ADDR_LIST", "129.57.86.54", 1);  // dcrb1 (for old CAEN)



    mainw=this;
    constructed=0;
    current_crate=0;
  //  char epics_record[20];
   // printf("hello 1\n");
   // fflush(stdout);

    crates_number=NCRATES;
    crate_names=hv_crate_names;
 /*
    for(int i=0;i<crates_number;i++){

     hvcrates[i]=new CrateButton(this, i);

     //hvb[0]->setObjectName(QString::fromUtf8(ename));
     hvcrates[i]->setText(QApplication::translate("MainWindow", crate_names[i], 0, QApplication::UnicodeUTF8));
     hvcrates[i]->setGeometry(QRect(10,20+i*30, 110, 30));

     connect(hvcrates[i], SIGNAL(clicked()), hvcrates[i], SLOT(s_crateDisplay()));
    }

   // connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(s_destructor()));
*/


    bback = new  QPushButton(this);
    bback->setText("BACK");
    bback->setGeometry(QRect(HvCrate::TABLE_NEW_XBASE-HvCrate::X_BOARDS, HvCrate::TABLE_YBASE+600, 110, 30));
    bback->setVisible(false);
    connect(bback, SIGNAL(clicked()), this, SLOT(s_back()));

    s_back();

    //connect(ui->pushButton, SIGNAL(clicked()), HvCrate, SLOT(s_destructor()));

    //ui->qcaedit->setVariableNameAndSubstitutions( "b_hv000_0_0_pwonoff" , 0, 0 );
    //ui->qcapush->setVariableNameAndSubstitutions( "b_hv000_0_0_pwonoff" , 0, 0 );
    //ui->qcapush->activate();
   // ui->qcaspin->setVariableNameAndSubstitutions( "b_hv000_0_0_pwonoff" , 0, 0 );
   //ggg=new QCaComboBox("b_hv000_0_0_pwonoff", this);
   // QCaLineEdit *ggg=new QCaLineEdit("b_hv000_0_0_pwonoff", this);
    //ggg->setGeometry(QRect(130, 140, 67, 36));
    //ggg->setVariableNameAndSubstitutions( "b_hv000_0_0_pwonoff" , 0, 0 );
    //ggg->activate();
/*
     ui->qcacombo->setVariableNameAndSubstitutions( "b_hv000_0_0_pwonoff" ,0, 0 );
     ui->qcacombo->setSubscribe(true);
     //ui->qcacombo->setVariableName( "b_hv000_0_1_pwonoff" ,1 );
    ui->qcacombo->activate();
*/
    ///QCaInteger* qca = (QCaInteger*)ui->qcacombo->getQcaItem(0); // example of direct reading
     ///      qca->singleShotRead(); // example of direct reading; cont

//ui->qcacombo->singleShotRead();

/*
    QCaLabel* qcal_1 = new QCaLabel( "b_hv001_2_0", this );
    qcal_1->setGeometry(QRect(130, 120, 57, 15));
    qcal_1->setFrameShape(QFrame::Box);
    qcal_1->activate();

    qcal_1->setVariableNameAndSubstitutions( "b_hv000_0_1" , 0, 0 );
*/

//    ui->qcal_1->setVariableNameAndSubstitutions( "b_hv001_2_0" , "b_hv001_2_0", 1 );

    ui->qcal_1->setVisible(false);
    ui->qcal_2->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->qcacombo->setVisible(false);
    ui->qcapush->setVisible(false);
    ui->qcaspin->setVisible(false);
    ui->qcaedit->setVisible(false);
    ui->label23->setVisible(false);
}

//=================================================================
MainWindow::~MainWindow()
{

    delete ui;
}
//=================================================================
void MainWindow::s_back()
{

  if(MainWindow::constructed){
   if(MainWindow::hvcrate->warning_db->isVisible()) return;
   delete MainWindow::hvcrate;
  }

  for(int i=0;i<crates_number;i++){

      hvcrates[i]=new CrateButton(this, i, hv_crate_types[i]);

   //hvb[0]->setObjectName(QString::fromUtf8(ename));
   hvcrates[i]->setText( crate_names[i]);
   hvcrates[i]->setGeometry(QRect(HvCrate::TABLE_NEW_XBASE-HvCrate::X_BOARDS ,HvCrate::TABLE_YBASE+i*40, 110, 40));
   hvcrates[i]->setVisible(true);
   connect(hvcrates[i], SIGNAL(clicked()), hvcrates[i], SLOT(s_crateDisplay()));
   connect(hvcrates[i], SIGNAL(clicked()), this, SLOT(s_crateListDelete()));
  }

  bback->setVisible(false);

 // connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(s_destructor()));

}
//=================================================================
void MainWindow::s_crateListDelete()
{
    for(int i=0;i<crates_number;i++){

     delete hvcrates[i];

    }

}
//=================================================================
void MainWindow::s_destructor()
{


}
//=================================================================
void CrateButton::s_crateDisplay(){

   // if(MainWindow::current_crate == this)return;

    MainWindow::bback->setVisible(true);
    if(MainWindow::constructed){

      //  delete MainWindow::HvCrate;
    }

    //char *fsm_names1[]={"b_hv000_0_P0", "b_hv000_2_P0"};
  //  char **fsm_names=board_names_by_crate[crate_index];
    //int types[]={HvCrate::CAEN_A1535, HvCrate::CAEN_A1535};
//    int *types=board_types_by_crate[crate_index];
    //HvCrate HvCrate(fsm_names, 2);
    int size=NBOARDS;/// my_n2: sizeof(board_names_by_crate[crate_index])/sizeof(board_names_by_crate[crate_index][0]);
 /// printf("ggg %d\n", size);fflush(stdout);

/// my_n2:    MainWindow::hvcrate=new HvCrate(board_types_by_crate[crate_index], board_names_by_crate[crate_index],
///                size, crate_type, crate_index);
    MainWindow::hvcrate=new HvCrate(fun_board_types_by_crate(crate_index), fun_board_names_by_crate(crate_index),
                    size, crate_type, crate_index);/// my_n2:

    MainWindow::constructed=1;
    MainWindow::current_crate=this;

};
//=================================================================
void HvCrate::s_destructor(){
/*
     for(int j=0;j<PARS_NUM;j++){
         delete label_hvpar[j];
     }
 */

     current_board->destruct();
/*
    for(int i=0;i<chs_num;i++){
     for(int j=0;j<PARS_READ;j++){
         delete hvch[i].hvpar_read[j];
     }

     for(int j=0;j<PARS_SWITCH;j++){
         delete hvch[i].hvpar_switch[j];
     }

     for(int j=0;j<PARS_SET;j++){
         delete hvch[i].hvpar_set[j];
     }
    } //i
*/

    for(int i=0;i<boards_number;i++){
     delete hvb[i];
    }
    delete HvCrate::lab_cr_name;
    delete timer_db;
    delete warning_db;
    delete unit_not_connected; // my_n: hbeat
    delete HvCrate::bturnonallchannels;
    delete HvCrate::bturnoffallchannels;
    delete HvCrate::bcrateon;
    delete HvCrate::bcrateoff;
    delete HvCrate::bsetallchannels;

    HvCrate::lab_cr_name=0;
    HvCrate::bturnonallchannels=0;
    HvCrate::bturnoffallchannels=0;
    HvCrate::bcrateon=0;
    HvCrate::bcrateoff=0;
    HvCrate::bsetallchannels=0;
    delete HvCrate::bstore;
    delete HvCrate::bload;

}
//========================================================================================
HvCrate::~HvCrate(){

    s_destructor();


}
//========================================================================================
void *cb_thread_store_config(void *arg){

  char *tmp=(char *)arg;
 //
  MainWindow::hvcrate->s_store_process(tmp);
  MainWindow::hvcrate->activate_after_config();

 // MainWindow::hvcrate->db_under_processing=0;
  return 0;
}
//========================================================================================
void *cb_thread_load_config(void *arg){

  char *tmp=(char *)arg;
 //

  MainWindow::hvcrate->s_load_process(tmp);
  MainWindow::hvcrate->activate_after_config();


  return 0;
}//========================================================================================
void *cb_thread_crateonoff(void *arg){

  int ison=*((int *)arg);
 //
    MainWindow::hvcrate->s_crateonoff(ison);


  return 0;
}

//========================================================================================
void HvCrate::activate_after_config(){

  MainWindow::hvcrate->warning_db->setVisible(false);
  MainWindow::hvcrate->bstore->setEnabled(true);
  MainWindow::hvcrate->bload->setEnabled(true);

}
//========================================================================================
void HvCrate::cb_update_db(){

//printf("1\n"); fflush(stdout);
 char tmp[256];  // temporal
 // if( !db_under_processing){
 //   db_under_processing=1;

    if(need_db_store){
      need_db_store=0;

      QFileDialog fdialog(mainw);
      fdialog.setFileMode(QFileDialog::AnyFile);
      fdialog.setNameFilter(tr("config files (*.cfg)"));
      fdialog.setViewMode(QFileDialog::Detail);
      fdialog.setDirectory(".");
      QStringList fileNames;
      if (fdialog.exec()){
          fileNames = fdialog.selectedFiles();
         // printf("1\n"); fflush(stdout);
       strcpy(tmp, fileNames[0].toAscii());
       int rval = pthread_create(&thread_config, NULL, cb_thread_store_config, tmp );
       if (rval != 0) {
        perror("Creating the thread_config failed");
        exit(1);
       }
       //s_store_process(tmp);
      }
      else MainWindow::hvcrate->activate_after_config();
    }
    else if(need_db_load){
      need_db_load=0;

      QFileDialog fdialog(mainw);
      fdialog.setFileMode(QFileDialog::ExistingFile);
      fdialog.setNameFilter(tr("config files (*.cfg)"));
      fdialog.setViewMode(QFileDialog::Detail);
      fdialog.setDirectory(".");
      QStringList fileNames;
      if (fdialog.exec()){
          fileNames = fdialog.selectedFiles();
        strcpy(tmp, fileNames[0].toAscii());
       int rval = pthread_create(&thread_config, NULL, cb_thread_load_config, tmp );
       if (rval != 0) {
        perror("Creating the thread_config failed");
        exit(1);
       }
        //s_load_process(tmp);
      }
      else MainWindow::hvcrate->activate_after_config();
    }
   // warning_db->setVisible(false);
   // under_processing=0;
   // bstore->setEnabled(true);
   // bload->setEnabled(true);
    // to cancell clicks during waiting:

  //}



}

//========================================================================================
HvCrate::HvCrate(int *types, char **fsm_names, int boards_number, int type, int crate_index) :  QObject(){

    QFont font;
    font.setBold(true);
    cr_index=crate_index;
    lab_cr_name=new QLabel(mainw);
    lab_cr_name->setText(hv_crate_names[cr_index]);
    lab_cr_name->setGeometry(QRect(TABLE_XBASE+600, HvCrate::TABLE_YBASE, 90, 30));
    lab_cr_name->setStyleSheet("QLabel { qproperty-alignment: AlignCenter;  }");
    lab_cr_name->setStyleSheet("QLabel { background : darkGrey; border: 1px solid black ; color : white; qproperty-alignment: AlignCenter;}");
 lab_cr_name->setFont(font);
    lab_cr_name->setVisible(true);


    HvCrate::crate_type=type;
    HvCrate::current_board=0;
    HvCrate::constructed=0;

    db_under_processing=0;
    need_db_store=0;
    need_db_load=0;
    timer_db = new QTimer(this);
    connect(timer_db, SIGNAL(timeout()), this, SLOT(cb_update_db()));
    timer_db->start(rate_update_db*1000); // ms
    warning_db=new QLabel(mainw);
    warning_db->setGeometry(QRect(300, 200, 300, 200));
    warning_db->setFrameShape(QFrame::Box);
    warning_db->setText("wait until \n configuration storing/loading \n is finished \n (takes a few minutes)");
    warning_db->setVisible(false);   /// this->boards_number=boards_number;
 //   warning_db->setStyleSheet("QLabel { background : red; border: 1px solid black ; color : black; }");
    warning_db->setStyleSheet("QLabel { qproperty-alignment: AlignCenter;  }");
    QFont font_db = warning_db->font();
    font_db.setPointSize(14);
    warning_db->setFont(font_db);
//------------------------------------------------ my_n2:
    get_crate_ioc_type(crate_index); // my_n2:
    slot_label=QString(slot_labels_in_record_name_by_ioc_type[crate_ioc_type]);
    channel_label=QString(channel_labels_in_record_name_by_ioc_type[crate_ioc_type]);
/*--------------------- my_n: hbeat-------------------*/
    unit_not_connected=new QLabel(mainw);
    unit_not_connected->setGeometry(QRect(HvCrate::TABLE_NEW_XBASE-HvCrate::X_BOARDS+130, HvCrate::TABLE_YBASE+600, 550, 30));
    //setGeometry(QRect(HvCrate::TABLE_XBASE-HvCrate::X_BOARDS, HvCrate::TABLE_YBASE+600, 110, 30));
    unit_not_connected->setFrameShape(QFrame::Box);
    //unit_not_connected->setText("wait until \n configuration storing/loading \n is finished \n (takes a few minutes)");

    unit_not_connected->setVisible(false);   /// this->boards_number=boards_number;
    unit_not_connected->setStyleSheet("QLabel { qproperty-alignment: AlignCenter;  }");
    unit_not_connected->setFont(font_db);
/*-----------------------------------------*/

    char ename[81];//temporal
    int bn=0;
    for(int i=0;i<boards_number;i++){
     if(strlen(fsm_names[i])==0)continue;
      bn++;
     strcpy(ename,fsm_names[i]);
     if(types[i] == CAEN_A1535){

      hvb[i]=new BoardButtonCaenA1535(mainw, ename);
     }
     if(types[i] == CAEN_A944){

      hvb[i]=new BoardButtonCaenA944(mainw, ename);
     }
     else if(types[i] == WMPOD_HV){
      hvb[i]=new BoardButtonWMpod(mainw, ename);
     }
     else if(types[i] == WMPOD_LV){
      hvb[i]=new BoardButtonWMpodLv(mainw, ename);
     }
      this->boards_number=bn;
     //hvb[0]->setObjectName(QString::fromUtf8(ename));

     char tmp[100];
     sprintf(tmp,"board %02d", hvb[i]->board);
     hvb[i]->setVisible(true);
     hvb[i]->setText( tmp);
     hvb[i]->setGeometry(QRect(HvCrate::TABLE_NEW_XBASE-HvCrate::X_BOARDS,HvCrate::TABLE_YBASE+i*30, 110, 30));

     connect(hvb[i], SIGNAL(clicked()), hvb[i], SLOT(s_boardDisplay()));
    }

    HvCrate::bturnonallchannels= new QPushButton(mainw);
    HvCrate::bturnonallchannels->setGeometry(QRect(TABLE_XBASE+600, HvCrate::TABLE_YBASE+HvCrate::SHIFT_Y, 90, 30));
    HvCrate::bturnonallchannels->setVisible(true);
    HvCrate::bturnonallchannels->setText("turn on board\n all channels");
    connect(HvCrate::bturnonallchannels, SIGNAL(clicked()), this, SLOT(s_turnonallchannels()));

    HvCrate::bturnoffallchannels= new QPushButton(mainw);
    HvCrate::bturnoffallchannels->setGeometry(QRect(TABLE_XBASE+600, HvCrate::TABLE_YBASE+HvCrate::SHIFT_Y+30, 90, 30));
    HvCrate::bturnoffallchannels->setVisible(true);
    HvCrate::bturnoffallchannels->setText("turn off board\n all channels");
    connect(HvCrate::bturnoffallchannels, SIGNAL(clicked()), this, SLOT(s_turnoffallchannels()));

    HvCrate::bcrateon= new QPushButton(mainw);
    HvCrate::bcrateon->setGeometry(QRect(TABLE_XBASE+600, HvCrate::TABLE_YBASE+HvCrate::SHIFT_Y+80, 90, 30));
    HvCrate::bcrateon->setVisible(true);
    HvCrate::bcrateon->setText("turn on \n whole crate");
    connect(HvCrate::bcrateon, SIGNAL(clicked()), this, SLOT(s_crateon()));

    HvCrate::bcrateoff= new QPushButton(mainw);
    HvCrate::bcrateoff->setGeometry(QRect(TABLE_XBASE+600, HvCrate::TABLE_YBASE+HvCrate::SHIFT_Y+110, 90, 30));
    HvCrate::bcrateoff->setVisible(true);
    HvCrate::bcrateoff->setText("turn off \n whole crate");
    connect(HvCrate::bcrateoff, SIGNAL(clicked()), this, SLOT(s_crateoff()));


    HvCrate::bsetallchannels= new QPushButton(mainw);
    HvCrate::bsetallchannels->setGeometry(QRect(TABLE_XBASE+600, HvCrate::TABLE_YBASE+HvCrate::SHIFT_Y+160, 90, 30));
    HvCrate::bsetallchannels->setVisible(true);
    HvCrate::bsetallchannels->setText("set \n all channels");
    connect(HvCrate::bsetallchannels, SIGNAL(clicked()), this, SLOT(setAllChannels()));

    HvCrate::bstore= new QPushButton(mainw);
    HvCrate::bstore->setGeometry(QRect(TABLE_XBASE+600, HvCrate::TABLE_YBASE+530, 90, 30));
    HvCrate::bstore->setVisible(true);
    HvCrate::bstore->setText("store crate\n configuration");
    connect(HvCrate::bstore, SIGNAL(clicked()), this, SLOT(s_store()));

    HvCrate::bload= new QPushButton(mainw);
    HvCrate::bload->setGeometry(QRect(TABLE_XBASE+600, HvCrate::TABLE_YBASE+560, 90, 30));
    HvCrate::bload->setVisible(true);
    HvCrate::bload->setText("load crate\n configuration");
    connect(HvCrate::bload, SIGNAL(clicked()), this, SLOT(s_load()));

     if(this->boards_number>0)hvb[0]->boardConnect();

}
//========================================================================================
void BoardButton::s_boardDisplay(){

if(MainWindow::hvcrate->warning_db->isVisible()) return;
MainWindow::hvcrate->unit_not_connected->setVisible(false);
 boardConnect();
//ggg->activate();
//ggg->setVariableNameAndSubstitutions( "b_hv000_0_0_pwonoff" , 0, 0 );
return;
}
//=================================================================
void BoardButton::destruct(){

    if(HvCrate::constructed==0)return;

    for(int j=0;j<HvCrate::params_num;j++){
        delete HvCrate::label_hvpar[j];
    }
    delete HvCrate::label_chnumber_title;
    delete HvCrate::label_log_name;
    for(int i=0;i<HvCrate::channels_num;i++){
     delete HvCrate::hvch[i].hbeat; // my_n:
     delete HvCrate::hvch[i].log_name;
     delete HvCrate::hvch[i].label_chnumber;
     for(int j=0;j<HvCrate::params_read;j++){
         delete HvCrate::hvch[i].hvpar_read[j];
     }

     for(int j=0;j<HvCrate::params_switch;j++){
         delete HvCrate::hvch[i].hvpar_switch[j];
     }

     for(int j=0;j<HvCrate::params_set;j++){
         delete HvCrate::hvch[i].hvpar_set[j];
     }
    } //i


}
//=================================================================
void BoardButton::construct(){


//printf("construct\n");fflush(stdout);
    HvCrate::label_chnumber_title=new QLabel(mainw);
    HvCrate::label_chnumber_title->setText("#");
    HvCrate::label_chnumber_title->setGeometry(QRect(HvCrate::TABLE_NEW_XBASE-30, HvCrate::TABLE_YBASE, 30, 20));
    HvCrate::label_chnumber_title->setFrameShape(QFrame::Box);
    HvCrate::label_chnumber_title->setVisible(true);

    HvCrate::label_log_name=new QLabel(mainw);
    HvCrate::label_log_name->setText("log.name");
    HvCrate::label_log_name->setGeometry(QRect(HvCrate::TABLE_NEW_XBASE, HvCrate::TABLE_YBASE, HvCrate::TABLE_XCELLLOG, 20));
    HvCrate::label_log_name->setFrameShape(QFrame::Box);
    HvCrate::label_log_name->setVisible(true);

    for(int j=0;j<HvCrate::params_read;j++){

        HvCrate::label_hvpar[j] = new QLabel(mainw);
        HvCrate::label_hvpar[j]->setText( params_labels_caen[j]);
        HvCrate::label_hvpar[j]->setGeometry(QRect(HvCrate::TABLE_XBASE+j*60, HvCrate::TABLE_YBASE, 60, 20));
        HvCrate::label_hvpar[j]->setFrameShape(QFrame::Box);
        HvCrate::label_hvpar[j]->setVisible(true);
    }

    for(int j=0;j<HvCrate::params_switch;j++){

        HvCrate::label_hvpar[j+HvCrate::params_read] = new QLabel(mainw);
        HvCrate::label_hvpar[j+HvCrate::params_read]->setText(params_labels_caen[j+HvCrate::params_read]);
        HvCrate::label_hvpar[j+HvCrate::params_read]->setGeometry(QRect(HvCrate::TABLE_XBASE+(j+HvCrate::params_read)*60, HvCrate::TABLE_YBASE, 60, 20));
        HvCrate::label_hvpar[j+HvCrate::params_read]->setFrameShape(QFrame::Box);
        HvCrate::label_hvpar[j+HvCrate::params_read]->setVisible(true);
    }

    for(int j=0;j<HvCrate::params_set;j++){
        HvCrate::label_hvpar[j+HvCrate::params_read+HvCrate::params_switch] = new QLabel(mainw);
        HvCrate::label_hvpar[j+HvCrate::params_read+HvCrate::params_switch]->setText( params_labels_caen[j+HvCrate::params_read+HvCrate::params_switch]);
        HvCrate::label_hvpar[j+HvCrate::params_read+HvCrate::params_switch]->setGeometry(QRect(HvCrate::TABLE_XBASE+(j+HvCrate::params_read+HvCrate::params_switch)*60, HvCrate::TABLE_YBASE, 60, 20));
        HvCrate::label_hvpar[j+HvCrate::params_read+HvCrate::params_switch]->setFrameShape(QFrame::Box);
        HvCrate::label_hvpar[j+HvCrate::params_read+HvCrate::params_switch]->setVisible(true);
    }



    for(int i=0;i<HvCrate::channels_num;i++){
        int i10=i+1;

/*--------------------- my_n: adding heart beat ---------------------------------------------------------------------*/

        QString qs=QString( "some_name" );
        HvCrate::hvch[i].hbeat=new MyQCaLabel(qs,(QWidget *) mainw, hbeat_param_number, i );
        connect(HvCrate::hvch[i].hbeat, SIGNAL(dbValueChanged(QString)), HvCrate::hvch[i].hbeat, SLOT(s_dbValueChanged(QString)) );
        HvCrate::hvch[i].hb_isactivated_hbeat=0;
        HvCrate::hvch[i].hbeat  ->setVisible(false);

/*-------------------------------------------------------------------------------------------------------------------*/
        qs=QString( "some_name" );
        HvCrate::hvch[i].log_name=new MyQCaLabel(qs,(QWidget *) mainw, -1, i );
      //  connect(HvCrate::hvch[i].log_name, SIGNAL(dbValueChanged(QString)), HvCrate::hvch[i].log_name, SLOT(s_dbValueChanged(QString)) );
        HvCrate::hvch[i].hb_isactivated_log_name=0;
        HvCrate::hvch[i].log_name->setVisible(true);
        QFont font_1 = HvCrate::hvch[i].log_name->font();
        font_1.setPointSize(7);
        HvCrate::hvch[i].log_name->setFont(font_1);
        HvCrate::hvch[i].log_name->setGeometry(QRect(HvCrate::TABLE_NEW_XBASE, HvCrate::TABLE_YBASE+i10*20, HvCrate::TABLE_XCELLLOG, 20));
        HvCrate::hvch[i].log_name->setFrameShape(QFrame::Box);
/*-------------------------------------------------------------------------------------------------------------------*/
        HvCrate::hvch[i].label_chnumber=new QLabel(mainw);
        HvCrate::hvch[i].label_chnumber->setText(QString::number(i));
        HvCrate::hvch[i].label_chnumber->setGeometry(QRect(HvCrate::TABLE_NEW_XBASE-30, HvCrate::TABLE_YBASE+i10*20, 30, 20));
        HvCrate::hvch[i].label_chnumber->setFrameShape(QFrame::Box);
        HvCrate::hvch[i].label_chnumber->setVisible(true);

     for(int j=0;j<HvCrate::params_read;j++){
         QString qs=QString( "some_name" );
         HvCrate::hvch[i].hvpar_read[j]=new MyQCaLabel(qs,(QWidget *) mainw, j,i ); // my_n
         //QFont font_1 = HvCrate::hvch[i].hvpar_read[j]->font();
         //font_1.setPointSize(7);
         //HvCrate::hvch[i].hvpar_read[j]->setFont(font_1);

         connect(HvCrate::hvch[i].hvpar_read[j], SIGNAL(dbValueChanged(QString)), HvCrate::hvch[i].hvpar_read[j], SLOT(s_dbValueChanged(QString)) );
         HvCrate::hvch[i].hb_isactivated[j]=0;

//         HvCrate::hvch[i].hvpar_read[j]->setVisible(false);

         HvCrate::hvch[i].hvpar_read[j]->setGeometry(QRect(HvCrate::TABLE_XBASE+j*60, HvCrate::TABLE_YBASE+i10*20, 60, 20));
         HvCrate::hvch[i].hvpar_read[j]->setFrameShape(QFrame::Box);

     }

     for(int j=0;j<HvCrate::params_switch;j++){
         HvCrate::hvch[i].hvpar_switch[j]=new MyQCaComboBox( "some_name" , mainw, j );
         HvCrate::hvch[i].hvpar_switch[j]->setSubscribe(true);
         connect(HvCrate::hvch[i].hvpar_switch[j], SIGNAL(dbValueChanged(qlonglong)), HvCrate::hvch[i].hvpar_switch[j], SLOT(s_dbValueChanged(qlonglong)) );
         HvCrate::hvch[i].hb_isactivated[j+HvCrate::params_read]=0;
         if(HvCrate::crate_type==CrateButton::WMPOD && j==0){
             HvCrate::hvch[i].hvpar_switch[j]->insertItem(0, "OFF");
             HvCrate::hvch[i].hvpar_switch[j]->insertItem(1, "ON");
         }
  //       HvCrate::hvch[i].hvpar_switch[j]->setVisible(false);

         HvCrate::hvch[i].hvpar_switch[j]->setGeometry(QRect(HvCrate::TABLE_XBASE+(j+HvCrate::params_read)*60, HvCrate::TABLE_YBASE+i10*20, 80, 20));

     }

     for(int j=0;j<HvCrate::params_set;j++){
         HvCrate::hvch[i].hvpar_set[j]=new MyQCaLineEdit( "some_name" , mainw, j, i );
         HvCrate::hvch[i].hb_isactivated[j+HvCrate::params_read+HvCrate::params_switch]=0;

    //     HvCrate::hvch[i].hvpar_set[j]->setVisible(false);

         HvCrate::hvch[i].hvpar_set[j]->setGeometry(QRect(HvCrate::TABLE_XBASE+(j+HvCrate::params_read+HvCrate::params_switch)*60, HvCrate::TABLE_YBASE+i10*20, 60, 20));

     }
    } //i

HvCrate::constructed=1;

}
//========================================================================================
//========================================================================================
void HvCrate::s_crateon(){
if(MainWindow::hvcrate->warning_db->isVisible()) return;

int ison=1;
int rval = pthread_create(&thread_crateonoff, NULL, cb_thread_crateonoff, &ison );
if (rval != 0) {
 perror("Creating the thread_config failed");
 exit(1);
}


}
//========================================================================================
void HvCrate::s_crateoff(){
if(MainWindow::hvcrate->warning_db->isVisible()) return;
int ison=0;
int rval = pthread_create(&thread_crateonoff, NULL, cb_thread_crateonoff, &ison );
if (rval != 0) {
 perror("Creating the thread_config failed");
 exit(1);
}


}
//========================================================================================
void HvCrate::s_crateonoff(int ison){
    char tmp[300]; //temporal
   // float par;

    int size=0;
    int counter=0;
    int pointer=0;

    char *t;

  QString en;
  for(int i=0;i<boards_number;i++){
    for(int ch=0;ch< hvb[i]->BOARD_CHS_NUM;ch++){
      for(int j=0;j<HvCrate::params_switch;j++){

          en=hvb[i]->epics_name+"_"+channel_label+QString().setNum(ch)+
          QString(hvb[i]->epics_suffixes_parameters[j+HvCrate::params_read]);
          strcpy(tmp,"caput -w 2 ");
          strcat(tmp,en.toAscii());
          if(!ison) strcat(tmp," 0 > /dev/null 2>&1;");
          else strcat(tmp," 1 > /dev/null 2>&1;");

          size+=strlen(tmp);
          if(counter==0)t=(char *)malloc(size+1);
          else t=(char *)realloc(t,size+1);

          strcpy(t+pointer,tmp);
          pointer=size;

         // printf("size=%d %d \n",(int)size, (int)strlen(t) );
          counter++;

//           printf("%s\n", tmp); fflush(stdout);

       }
    }
  }
  system(t);
  printf("end of crate switching\n");fflush(stdout);
  //printf("end of crate switching %s\n",t);fflush(stdout);

}
//========================================================================================
void HvCrate::s_store(){
  need_db_store=1;
  warning_db->setVisible(true);
  warning_db->setStyleSheet("QLabel { background : red; border: 1px solid black ; color : black; }");
  warning_db->raise();
  bstore->setEnabled(false);
  bload->setEnabled(false);

}
//========================================================================================
//========================================================================================
void HvCrate::s_store_process(char *store_file){
    char tmp[300], tmp1[200], spar[30]; //temporal
    float par;

  FILE *fp=fopen(store_file,"w+");
  FILE *fp1=fopen("temp_config.txt","w+");
  if(!fp){printf("config files open failure (store fp) \n"); exit(1);}
  if(!fp1){printf("config files open failure (store fp1) \n"); exit(1);}

  QString en;
  for(int i=0;i<boards_number;i++){
    for(int ch=0;ch< hvb[i]->BOARD_CHS_NUM;ch++){
      for(int j=0;j<HvCrate::params_set;j++){
         //for(int j=0;j<ps.size()-1;j++){
          en=hvb[i]->epics_name+"_"+channel_label+QString().setNum(ch)+
          QString(hvb[i]->epics_suffixes_parameters[j+HvCrate::params_read+HvCrate::params_switch]);
          //strcpy(tmp,"echo > ");
          strcpy(tmp,"caget -w 0.5 ");
          strcat(tmp,en.toAscii());
          strcat(tmp," > ");
          strcat(tmp,"temp_config.txt 2>&1");
//           printf("%s\n", tmp); fflush(stdout);
          //system("echo >");
          int ret=system(tmp);
 //         printf("0\n"); fflush(stdout);
//          fseek(fp1,0, SEEK_SET);
          fclose(fp1);
          fp1=fopen("temp_config.txt","r");

          if(!fp1){printf("config files open failure (store: fp1_) \n"); exit(1);}


//          printf("1\n"); fflush(stdout);
          fgets(tmp, 300, fp1);
          //printf("22 =%s ret=%d\n", tmp, ret); fflush(stdout);
          if(ret){//printf("2 =%s\n", tmp); fflush(stdout);
            if(j==0)break;
            else {par=-1;} /// i.e. negative
          }
          else {
//          printf("2 =%s\n", tmp); fflush(stdout);
          sscanf(tmp,"%s %s",tmp1,spar);
          par=atof(spar);
          }
 //         printf("= %s %.2f\n",tmp, par); fflush(stdout);
          if(j==0)fprintf(fp,"%d.%d %.2f ", hvb[i]->board, ch, par);
          else if(j<(HvCrate::params_set-1))fprintf(fp,"%.2f ", par);
          else fprintf(fp,"%.2f \n", par);
       }
    }
  }
  printf("end of storing\n");fflush(stdout);
  fclose(fp);
  fclose(fp1);

}
//========================================================================================
void HvCrate::s_load(){
  need_db_load=1;
  warning_db->setVisible(true);
  warning_db->setStyleSheet("QLabel { background : red; border: 1px solid black ; color : black; }");
  warning_db->raise();
  bload->setEnabled(false);
  bstore->setEnabled(false);
//  QFont font_db = warning_db->font();
//  font_db.setPointSize(72);
//  warning_db->setFont(font_db);
}

//========================================================================================
void HvCrate::s_load_process(char *load_file){

 FILE *fp;
// fp=fopen("/misc/clas/clas12/R3.14.12.3/epicsqt-1.1.2-src/hv_control/db_config.txt","r");
 //fp=fopen("db_config.txt","r");
  fp=fopen(load_file,"r");
 if(!fp){printf("config files open failure (load fp) \n"); exit(1);}

 char line[400];
 int br;
 int ch;

 //char pars[20][15]; // 20 params, 20 characters long
 while(fgets(line,400,fp)){
  // strcat(line, " ");
   for(int i=0;i<(int)strlen(line);i++){
       if(line[i]==10 || line[i]==13)line[i]=0;
   }
   strcat(line, " ");

     string s=string(line);
   size_t pos = 0;
   //std::string token;
   size_t pos1=0;
   vector<string> ps;
  // printf("loading 0\n"); fflush(stdout);
   while ((pos = s.find(" ", pos1)) != std::string::npos) {
     // printf("loading 1\n"); fflush(stdout); sleep(1);
     if(pos1 != 0 && pos==pos1){pos1=pos+1;continue;}
     //printf("loading 2\n"); fflush(stdout); sleep(1);
     ps.push_back(s.substr(pos1, pos));
     pos1=pos+1;
   }
 //  printf("loading %d %d\n", strlen(s.c_str()), pos1); fflush(stdout);
  // if((pos1+1) != strlen(s.c_str()))
     //  ps.push_back(s.substr(pos1));
  // printf("loading size=%d\n",ps.size()); fflush(stdout);
   for(size_t i=0;i<ps.size();i++){
     if(i==0){
      pos = ps[i].find(".", 0);
      br=atoi((ps[i].substr(0,pos)).c_str());
      ch=atoi((ps[i].substr(pos+1)).c_str());
     }
     else{

         load_db( br, ch, i, ps );
     }
   }
 }


 fclose(fp);
// printf("end of loading\n");fflush(stdout);
}
//========================================================================================
void HvCrate::load_db(int br, int ch, int i, vector<string> &ps ){

  static int previous_br=-1;
//  static int found_ch_number=-1;
  char tmp[300]; //temporal
  float par=atof(ps[i].c_str());
  static BoardButton *found_br;
  QString en;
  if(br != previous_br){
      found_br=db_board_finding(br);
      previous_br=br;
  }

  if(!found_br){}// temporal : error processing
  else{
//     for(int j=0;j<HvCrate::params_set;j++){
      //for(int j=0;j<ps.size()-1;j++){
       en=found_br->epics_name+"_"+channel_label+QString().setNum(ch)+
       QString(found_br->epics_suffixes_parameters[i-1+HvCrate::params_read+HvCrate::params_switch]);


       strcpy(tmp,"caput -w 6 ");
       strcat(tmp,en.toAscii());
       sprintf(tmp,"%s %.2f > /dev/null",tmp, par);
       if(par>=0) system(tmp);
     //  printf("loading %s %f\n", tmp, par); fflush(stdout);
    // }
  }
}
//========================================================================================
BoardButton * HvCrate::db_board_finding(int br){
  for(int i=0;i<boards_number;i++){
    if(HvCrate::hvb[i]->board==br)return HvCrate::hvb[i];
  }

  return NULL;
}
//========================================================================================
void HvCrate::s_turnonallchannels(){
if(MainWindow::hvcrate->warning_db->isVisible()) return;
    int f_ch=HvCrate::first_channel;
   /// for(int i12=0;i12<2;i12++)  //
    for(int i=f_ch;i<(f_ch+HvCrate::channels_num);i++){
        int i10=i-f_ch;
     for(int j=0;j<HvCrate::params_switch;j++){
        // HvCrate::hvch[i10].hvpar_switch[0]->setCurrentIndex(1);
         emit HvCrate::hvch[i10].hvpar_switch[0]->activated (1);
     }
    }

}
//========================================================================================
void HvCrate::s_turnoffallchannels(){
if(MainWindow::hvcrate->warning_db->isVisible()) return;
    int f_ch=HvCrate::first_channel;
   /// for(int i12=0;i12<2;i12++)
    for(int i=f_ch;i<(f_ch+HvCrate::channels_num);i++){
        int i10=i-f_ch;
     for(int j=0;j<HvCrate::params_switch;j++){
      //   HvCrate::hvch[i10].hvpar_switch[0]->setCurrentIndex(0);
         emit HvCrate::hvch[i10].hvpar_switch[0]->activated (0);
     }
    }
    /*
    char tmp[400];
    QString qs=mainw->styleSheet();
    strcpy(tmp,qs.toAscii());
    printf("loading =====================================*********************************************%s\n", tmp); fflush(stdout);
*/
}
//========================================================================================
void BoardButton::boardConnect(){

    if(this==HvCrate::current_board)return;
    QFont font;
    if(HvCrate::current_board){
        font.setBold(false);
        //font.setWeight(50);
        HvCrate::current_board->setFont(font);
    }
    font.setBold(true);
    //font.setWeight(75);
    setFont(font);


int f_ch, chs_num;
if(board_part==0){f_ch=0; chs_num=BOARD_CHS_NUM;}
else if(board_part==1){f_ch=0; chs_num=BOARD_CHS_NUM/2;}
else if(board_part==2){f_ch=BOARD_CHS_NUM/2; chs_num=BOARD_CHS_NUM/2;}
//printf("5\n"); fflush(stdout);
destruct();
//printf("55\n"); fflush(stdout);
HvCrate::current_board=this; // should be after destruct()
HvCrate::channels_num=chs_num;
HvCrate::first_channel=f_ch;
construct();

QString en;
int i10;

//QString qs, qs2, qs3, qs4, qs5, qs6, qs7;

onoff_records.clear();


 for(int i=f_ch;i<(f_ch+HvCrate::channels_num);i++){
     i10=i-f_ch;
/*--------- heart beat" my_n: --------------------------*/
     en=epics_name+"_"+HvCrate::channel_label+QString().setNum(i)+QString(epics_hbeat_suffix_parameter); // my_n2
     HvCrate::hvch[i10].hbeat->setVariableNameAndSubstitutions( en , 0, 0 );
     if(HvCrate::hvch[i10].hb_isactivated_hbeat==0){
         HvCrate::hvch[i10].hbeat->activate();
         HvCrate::hvch[i10].hb_isactivated_hbeat=1;
     }
/*------------------------------------------------------*/
     en=epics_name+"_"+HvCrate::channel_label+QString().setNum(i)+QString(epics_log_name_suffix_parameter); // my_n2
     HvCrate::hvch[i10].log_name->setVariableNameAndSubstitutions( en , 0, 0 );
     if(HvCrate::hvch[i10].hb_isactivated_log_name==0){
         HvCrate::hvch[i10].log_name->activate();
         HvCrate::hvch[i10].hb_isactivated_log_name=1;
     }
/*------------------------------------------------------*/
  for(int j=0;j<HvCrate::params_read;j++){
      HvCrate::hvch[i10].hvpar_read[j]->setVisible(true);
      if(!strcmp(".L",epics_suffixes_parameters[j])){
       HvCrate::hvch[i10].hvpar_read[j]->setPrecision(0);
       HvCrate::hvch[i10].hvpar_read[j]->setUseDbPrecision(false);
      }
      en=epics_name+"_"+HvCrate::channel_label+QString().setNum(i)+QString(epics_suffixes_parameters[j]);// my_n2
/*
      char tmp[100]; // temporal
      strcpy(tmp, en.toAscii());
      printf("%s\n",tmp); fflush(stdout);
*/
      HvCrate::hvch[i10].hvpar_read[j]->setVariableNameAndSubstitutions( en , 0, 0 );
      //HvCrate::hvch[i10].hvpar_read[j]->setVariableName( en , 0);
      if(HvCrate::hvch[i10].hb_isactivated[j]==0){
          HvCrate::hvch[i10].hvpar_read[j]->activate();
          HvCrate::hvch[i10].hb_isactivated[j]=1;
      }
   // hvch[i].hvpar[j]=new QCaLabel( epics_record , this );
  }
  for(int j=0;j<HvCrate::params_switch;j++){
      HvCrate::hvch[i10].hvpar_switch[j]->setVisible(true);
      //if(!strcmp("L",epics_suffixes_parameters_caen[j+PARS_READ])){
      // HvCrate::hvch[i10].hvpar_switch[j]->setPrecision(0);
      // HvCrate::hvch[i10].hvpar_switch[j]->setUseDbPrecision(false);
      //}
      en=epics_name+"_"+HvCrate::channel_label+QString().setNum(i)+QString(epics_suffixes_parameters[j+HvCrate::params_read]);// my_n2
      char tmp[150]; //temporal
      strcpy(tmp, en.toAscii());
      onoff_records.push_back(string(tmp));
      //char tmp[100]; // temporal
//      strcpy(tmp, en.toAscii());
//      printf("%s\n",tmp); fflush(stdout);
     // HvCrate::hvch[i10].hvpar_switch[j]->setVariableName( en , 0 );
     HvCrate::hvch[i10].hvpar_switch[j]->setVariableNameAndSubstitutions( en , 0, 0 );
      if(HvCrate::hvch[i10].hb_isactivated[j+HvCrate::params_read]==0){
          HvCrate::hvch[i10].hvpar_switch[j]->activate();
          HvCrate::hvch[i10].hb_isactivated[j+HvCrate::params_read]=1;
      }
   // hvch[i].hvpar[j]=new QCaLabel( epics_record , this );
  }
  for(int j=0;j<HvCrate::params_set;j++){
      HvCrate::hvch[i10].hvpar_set[j]->setVisible(true);
      //if(!strcmp("L",epics_suffixes_parameters_caen[j+PARS_READ])){
      // HvCrate::hvch[i10].hvpar_switch[j]->setPrecision(0);
      // HvCrate::hvch[i10].hvpar_switch[j]->setUseDbPrecision(false);
      //}
      en=epics_name+"_"+HvCrate::channel_label+QString().setNum(i)+QString(epics_suffixes_parameters[j+HvCrate::params_read+HvCrate::params_switch]);// my_n2
      if(board_type==HvCrate::CAEN_A944 && j==1){  // old CAEN and i0set is modified in all channels of the board at once
       en=epics_name+"_"+QString().setNum(f_ch)+QString(epics_suffixes_parameters[j+HvCrate::params_read+HvCrate::params_switch]);

      }
      //char tmp[100]; // temporal
//      strcpy(tmp, en.toAscii());
//      printf("%s\n",tmp); fflush(stdout);
      //HvCrate::hvch[i10].hvpar_set[j]->setVariableName( en , 0 );
      HvCrate::hvch[i10].hvpar_set[j]->setVariableNameAndSubstitutions( en , 0, 0 );
      if(HvCrate::hvch[i10].hb_isactivated[j+HvCrate::params_read+HvCrate::params_switch]==0){
          HvCrate::hvch[i10].hvpar_set[j]->activate();
          HvCrate::hvch[i10].hb_isactivated[j+HvCrate::params_read+HvCrate::params_switch]=1;
      }
   // hvch[i].hvpar[j]=new QCaLabel( epics_record , this );
  }
 }


return;
}
//========================================================================================
int BoardButton::boardNameParser(char *epics_name){

char *tmp1, *tmp2, *epics_name1;

char tmpq[10]="_"; // my_n2:
strcat(tmpq,HvCrate::slot_label.toAscii()); // my_n2:


 tmp2=epics_name;
 while((tmp1=strstr(tmp2,"_P"))){
  tmp2=tmp1+1;
 }
 //printf("hello 12 tmp2=%s\n",(tmp2-1 +strlen("_P")));
 //fflush(stdout);
 board_part=atoi(tmp2-1+strlen("_P"));
 //printf("%d %s\n",board_part, tmp2-1);
 //fflush(stdout);
 *(tmp2-1)=0;
 //printf("hello 11\n");
 //fflush(stdout);
 epics_name1=(char *)malloc(strlen(epics_name)+1);
 strcpy(epics_name1, epics_name);

 tmp2=epics_name;
 while((tmp1=strstr(tmp2,tmpq))){ // my_n2: "_"
  tmp2=tmp1+1;
 }

 board=atoi(tmp2-1+strlen(tmpq)); // my_n2: "_"
 *(tmp2-1)=0;

/** my_n2:
 tmp2=epics_name;
 while((tmp1=strstr(tmp2,"_hv"))){
  tmp2=tmp1+1;
 }
 //printf("hello 1\n");
 //fflush(stdout);
 crate=atoi(tmp2-1+strlen("_hv"));
*/
 tmp2=epics_name1; tmp1=0;
 while((tmp1=strstr(tmp2,"/"))){
  tmp2=tmp1+1;
 }
 if(!tmp1)strcpy(epics_name,tmp2);

 tmp2=epics_name1; tmp1=0;
 while((tmp1=strstr(tmp2,"::"))){
  tmp2=tmp1+2;
 }
 if(!tmp1)strcpy(epics_name,tmp2);

 ///printf("%d %d %d %s\n",crate, board, board_part, epics_name);
 fflush(stdout);

 free(epics_name1);
return 0;

}
//===============================================================================================
BoardButtonCaenA1535::BoardButtonCaenA1535(QWidget *widget, char *ename) : BoardButton(widget, ename){
    HvCrate::params_read=3;
    HvCrate::params_switch=1;
    HvCrate::params_set=5;
    HvCrate::params_num=HvCrate::params_read+HvCrate::params_switch+HvCrate::params_set;
    HvCrate::channels_num=0;
    BOARD_CHS_NUM=24;
    epics_suffixes_parameters=epics_suffixes_parameters_caen;
    epics_log_name_suffix_parameter=epics_log_name_suffix_parameter_caen;
    board_type=HvCrate::CAEN_A1535;
}
//===============================================================================================
BoardButtonCaenA944::BoardButtonCaenA944(QWidget *widget, char *ename) : BoardButton(widget, ename){
    HvCrate::params_read=3;
    HvCrate::params_switch=1;
    HvCrate::params_set=5;
    HvCrate::params_num=HvCrate::params_read+HvCrate::params_switch+HvCrate::params_set;
    HvCrate::channels_num=0;
    BOARD_CHS_NUM=24;
    epics_suffixes_parameters=epics_suffixes_parameters_caen;
    epics_log_name_suffix_parameter=epics_log_name_suffix_parameter_caen;
    board_type=HvCrate::CAEN_A944;
}
//===============================================================================================
BoardButtonWMpod::BoardButtonWMpod(QWidget *widget, char *ename) : BoardButton(widget, ename){
    HvCrate::params_read=3;
    HvCrate::params_switch=1;
    HvCrate::params_set=5;
    HvCrate::params_num=HvCrate::params_read+HvCrate::params_switch+HvCrate::params_set;
    HvCrate::channels_num=0;
    BOARD_CHS_NUM=16;
    epics_suffixes_parameters=epics_suffixes_parameters_wmpodhv;
    epics_log_name_suffix_parameter=epics_log_name_suffix_parameter_mpod;
    board_type=HvCrate::WMPOD_HV;
}
//===============================================================================================
BoardButtonWMpodLv::BoardButtonWMpodLv(QWidget *widget, char *ename) : BoardButton(widget, ename){
    HvCrate::params_read=3;
    HvCrate::params_switch=1;
    HvCrate::params_set=5;
    HvCrate::params_num=HvCrate::params_read+HvCrate::params_switch+HvCrate::params_set;
    HvCrate::channels_num=0;
    BOARD_CHS_NUM=8;
    epics_suffixes_parameters=epics_suffixes_parameters_wmpodhv;
    epics_log_name_suffix_parameter=epics_log_name_suffix_parameter_mpod;
    board_type=HvCrate::WMPOD_HV;
}

//===============================================================================================

void MyQCaLabel::s_dbValueChanged(QString str){
/*
   static int first=1;

    QString qs1;
    qs1=text();
    if(first){
        first=0;
        setText("jjj");
    }
    if(qs1=="jjj")setText("kkk");
    else setText("jjj");
*/

///------SY 527-------
   /// 0  Channel present
   /// 1  Don't care
   /// 2  Don't care
   /// 3  Don't care
   /// 4  Don't care
   /// 5  Internal Trip (**)
   /// 6  Kill
   /// 7  Don't care
   /// 8  Vmax (*)
   /// 9  External Trip (**)
   /// 10  Overvoltage
   /// 11  Undervoltage
   /// 12  Overcurrent
   /// 13  Down
   /// 14  Up
   /// 15  Channel On

///---- SY1527-------
    /// Channel is on
    #define BIT_ON 1
    /// 1 Channel is ramping up
    #define BIT_RAMPUP 1 << 1
    /// 2 Channel is ramping down
    #define BIT_RAMPDOWN 1 << 2
    /// 3 Channel is in overcurrent
    #define BIT_OVERCUR 1 << 3
    /// 4 Channel is in overvoltage
    #define BIT_OVERVOLT 1 << 4
    /// 5 Channel is in undervoltage
    #define BIT_UNDERVOLT 1 << 5
    /// 6 Channel is in external trip
    #define BIT_EXTTRIP 1 << 6
    /// 7 Channel is in max V
    #define BIT_MAXVOLT 1 << 7
    /// 8 Channel is in external disable
    #define BIT_EXTDISABLED 1 << 8
    /// 9 Channel is in internal trip
    #define BIT_INTTRIP 1 << 9
    /// 10 Channel is in calibration error
    #define BIT_CALIBERROR 1 << 10
    /// 11 Channel is unplugged
    #define BIT_CHUNPLAGGED 1 << 11
    /// my:
    #define BIT_OFF 1 << 11

      //char tmp[100];
      //strcpy(tmp,str.toAscii());
/*-----------------------my_n: hbeat------------------------------------*/
   if(parameter==15 && ichannel==HvCrate::first_channel){
       int hbeat=(int)str.toFloat();
      //printf(" %d %s 1\n", hbeat, tmp );fflush(stdout);

      if((hbeat)==0){
         MainWindow::hvcrate->unit_not_connected->setVisible(false);
       }
      else if(hbeat==1){
        MainWindow::hvcrate->unit_not_connected->setVisible(true);
        MainWindow::hvcrate->unit_not_connected->setText("Crate is not connected");
      }
      else if(hbeat==2){
        MainWindow::hvcrate->unit_not_connected->setVisible(true);
        MainWindow::hvcrate->unit_not_connected->setText("Board is not connected");
      }
      else if(hbeat==3){
        MainWindow::hvcrate->unit_not_connected->setVisible(true);
        MainWindow::hvcrate->unit_not_connected->setText("Hardware error(s): IOC reboot may help in some cases");
      }
   }
/*----------------------------------------------------------------------*/


    ///printf(" %s 1\n", tmp);fflush(stdout);
    ///
    /*
   for(int j=0;j<HvCrate::params_read;j++){
       HvCrate::hvch[i].hvpar_read[j]
   }
*/

      QString qs;
    if(HvCrate::crate_type==CrateButton::CAEN_1527 && parameter==2){
       int status= str.toInt();
       for(int i=0;i<12;i++){

        if((1<<i) & status)qs=QString(statuses_1527[i]);

       }
       if(status==0)qs=QString("OFF");


       QPalette pal = this->palette();
       this->colorControl(qs, pal);
       this->setPalette(pal);
       this->setAutoFillBackground(true);


    //   char tmp[100];
    //   strcpy(tmp,qs.toAscii());
   //  printf("status=%d %s\n", status, tmp);fflush(stdout);

     //strcpy(tmp, en.toAscii());
     // istatuses_o_1527=(istatuses_1527)i;
     setText(qs);
    }
    else if(HvCrate::crate_type==CrateButton::CAEN_527 && parameter==2){


       int status= str.toInt();
       int j10=-1;
qs=QString("OFF");
       for(int i=15;i>=5;i--){
        j10++;
        //printf("status=%d\n", status);fflush(stdout);
        if(i==7 || i==6)continue;
        if((1<<i) & status){
            //printf("%d %d \n", i, j10);fflush(stdout);
            qs=QString(statuses_527[j10]);}

       }
// if(!((1<<15)& status))qs=QString("OFF");
    //   if(status==0)qs=QString("OFF");
       if(((1<<6)& status))qs=QString("Kill");
       if(((1<<13)& status))qs=QString("RDN");
       if(!((1<<0) & status))qs=QString("NotPrt");
     //strcpy(tmp, en.toAscii());

       QPalette pal = this->palette();
       this->colorControl(qs, pal);
       this->setPalette(pal);
       this->setAutoFillBackground(true);

       setText(qs);
     /// setText(str);

    }

};

//===============================================================================================

void MyQCaComboBox::s_dbValueChanged(qlonglong out){




    if(HvCrate::crate_type==CrateButton::WMPOD && parameter==0){

        ///printf("MyQCaComboBox::s_dbValueChanged  =%ld\n", out); fflush(stdout);
        ///setCurrentIndex(out);
    }

}
//===============================================================================================
void MyQCaLabel::colorControl(QString &qs, QPalette &pal){


    if(qs=="ON"){pal.setColor(QPalette::Background , QColor(0,255,0));}
    else if(qs=="RUP" || qs=="RDN"){
        pal.setColor(QPalette::Window , Qt::yellow );
        // printf("yellow\n");fflush(stdout);
    }
    else if(qs=="OFF"){
        pal.setColor(QPalette::Background , QColor(210,210,210) );
    }
    else {
         pal.setColor(QPalette::Background , QColor(255,100,100) );
       //  printf("red\n");fflush(stdout);
    }




}

///========================================================================================================
void	MyQCaLineEdit::mousePressEvent ( QMouseEvent * event ){

HvCrate::current_channel=ichannel;
HvCrate::current_parameter=parameter;
//if(event); // to use event
//printf("%d %d loading\n", ichannel, index_in_plot); fflush(stdout);

}
///========================================================================================================



void HvCrate::setAllChannels(){
    if(current_channel==-1)return;
    QString vs= hvch[current_channel].hvpar_set[current_parameter]->text();
 //   double v=vs.toFloat();
//QString vs= QString::number(v, 'g', 1);
 for(int i=0;i<HvCrate::channels_num;i++){

    //int i10=i+1;
    hvch[i].hvpar_set[current_parameter]->setText(vs);
    emit hvch[i].hvpar_set[current_parameter]->s_returnPressed();
 }

//printf("yellow\n");fflush(stdout);

}
///========================================================================================================



