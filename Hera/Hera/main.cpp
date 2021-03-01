#include <QCoreApplication>
#include <iostream>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfoList>
#include <QStringList>
#include "parammanager.h"
#include "visionmodule.h"
#include "log_slider.h"
#include "logreader_global.h"
#include "netreceive.h"
#include "netreceive_global.h"
#include "Windows.h"
#include "staticparams.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineOption inputOption(QStringList() << "i" << "I", "", "inputoption", "lr");
    QCommandLineOption logFileName("if", "", "logfilename", "");
    QCommandLineOption logFileDir("id", "", "logfiledir", "");
    QCommandLineOption logFileClip("ic");
    QCommandLineOption outputOption(QStringList() << "o" << "O", "", "outputoption", "lw");
    QCommandLineOption outFileName("of", "", "outfilename", "test.zlog");
    QCommandLineOption outFileDir("od", "", "outfiledir", "../bin/");

    QCommandLineOption visionPort("p", "", "visionport", "10020");

    QCommandLineParser parser;
    parser.addOption(inputOption);
    parser.addOption(logFileName);
    parser.addOption(logFileDir);
    parser.addOption(logFileClip);
    parser.addOption(outputOption);
    parser.addOption(outFileName);
    parser.addOption(outFileDir);
    parser.addOption(visionPort);

    parser.process(a);

//    qDebug() << parser.value(inputOption);
//    qDebug() << parser.value(logFileName);
//    qDebug() << parser.value(outputOption);
//    qDebug() << parser.value(outFileName);
//    qDebug() << parser.value(cameraNumber);

    VisionModule vm;
    if(parser.value(inputOption) == "nr") qDebug() << "Vision port is" << parser.value(visionPort).toInt();
    vm.vision_port = parser.value(visionPort).toInt();

    //output
    if (parser.value(outputOption) == "lw") {
        qDebug() << "The output device is logwriter.";
        vm.flag = 0;
    } else if (parser.value(outputOption) == "ns") {
        qDebug() << "The output device is netsend.";
        vm.flag = 1;
    } else {
        qDebug() << "Unknown command. You can get more imformation from README.md";
    }

    //input
    if (parser.value(inputOption) == "lr") {
        qDebug() << "The input device is logreader.";
        LogSlider ls;

        //判断读取log文件的方式
        int cycle1 = 1;
        int cycle2[10] = {1};
        int startIndex[10], duration[10];
        QStringList m_logfile;
        if (parser.isSet(logFileClip)) {//log剪切功能
            m_logfile = ZSS::LParamManager::instance()->allGroups();
            if (m_logfile.size() == 0)
                qDebug() << "Can not find \"logclip.ini\" or \"logclip.ini\" is empty.";
            cycle1 = m_logfile.size();
            for (int i=0 ;i<cycle1 ;i++) {
                QStringList allKeys = ZSS::LParamManager::instance()->allKeys(m_logfile[i]);
                cycle2[i] = allKeys.size()/2;
                for (int j=0; j<cycle2[i]; j++) {
                    QString strj = QString::number(j+1);
                    ZSS::LParamManager::instance()->loadParam(startIndex[j], m_logfile[i] + "/Start" + strj, 0);
                    ZSS::LParamManager::instance()->loadParam(duration[j], m_logfile[i] + "/Stop" + strj, 0);
                    duration[j] = duration[j] - startIndex[j];
                    if (duration[j] <= 0){
                        qDebug() << "Unavailable duration for point" << j << "in file" << m_logfile[i];
                        break;
                    }
                }
            }
            m_logfile.replaceInStrings("...", "/");
        } else if (parser.value(logFileDir) == NULL) {//单个log文件读取
            m_logfile.append(parser.value(logFileName));
        } else {//log批量读取
            QDir inputDir;
            inputDir.setPath(parser.value(logFileDir));
            inputDir.setFilter(QDir::Files);
            QFileInfoList logList = inputDir.entryInfoList();
            for (int k = 0; k<logList.size(); k++)
                m_logfile.append(parser.value(logFileDir).append("/") + logList.at(k).fileName());
            cycle1 = logList.size();
        }

        //读取文件
        for(int i=0; i<cycle1; i++) {
            QString m_realLogfile = m_logfile.at(i);
            m_realLogfile.replace("...", "/");//将文件路径恢复正常
            if (ls.loadFile(m_realLogfile)) {
                qDebug() << "Load success! The log file is" << m_logfile.at(i);
            } else {
                qDebug() << "Load failed! Please check your DIR or FILENAME.";
                break;
            }
            for (int j=0; j<(parser.isSet(logFileClip) ? cycle2[i] : 1); j++) {
                if (vm.flag == 0) {
                    QString outFileName = m_logfile.at(i);
                    int a = outFileName.lastIndexOf("/");
                        outFileName.replace(0, a+1, parser.value(outFileDir));
                    if (parser.isSet(logFileClip)) {
                        QString strj = QString::number(j);
                        vm.lw_v.setFileName(outFileName.replace(".log.gz", "_bp_" + strj + ".zlog"));
                        vm.lw_rfb.setFileName(outFileName.replace(".zlog","_rfb.zlog"));
                    } else {
                        vm.lw_v.setFileName(outFileName.replace("log.gz", "zlog"));
                        vm.lw_rfb.setFileName(outFileName.replace(".zlog","_rfb.zlog"));
                    }
                }
                int m_currentFrame = parser.isSet(logFileClip) ? startIndex[j] : 0;
                int size = parser.isSet(logFileClip) ? duration[j] + m_currentFrame : ls.m_player.packets.size();
                while (++m_currentFrame < size) {
                    Frame* packet = ls.m_player.packets.at(m_currentFrame);
                    if (packet->type == MESSAGE_BLANK) {
                        // ignore
                    } else if (packet->type == MESSAGE_UNKNOWN) {
                        std::cout << "Error unsupported or corrupt packet found in log file!" << std::endl;
                    } else if (packet->type == MESSAGE_SSL_VISION_2010 || packet->type == MESSAGE_SSL_VISION_2014) {
                        vm.parse((void *)packet->data.data(), packet->data.size());
                    } else if (packet->type == MESSAGE_SSL_REFBOX_2013) {
                        if (vm.flag == 0) {
                            QByteArray buffer;
                            buffer.append(packet->time);
                            buffer.append(packet->data.data());
                            vm.lw_rfb.write(buffer);
                        }
                    } else {
                        std::cout << "Error unsupported message type found in log file!" << std::endl;
                    }
                    if (parser.value(outputOption) == "ns") {
                        Sleep(1000/60);
                    }
                    std::cout << m_currentFrame + 1 << "/" << size << "\r";
                }
                std::cout << "\nfinished" << std::endl;
            }
        }
        qDebug() << "All tasks finished, please find results in" << parser.value(outFileDir);
    } else if (parser.value(inputOption) == "nr") {
        qDebug() << "The input device is netreceive.";
        NetReceive nr;
        if (vm.flag == 0) {
            vm.lw_v.setFileName(parser.value(outFileName));
//            vm.lw_rfb.setFileName(parser.value(outFileName).replace(".zlog", "_rfb.zlog"));
//            不需要裁判盒信息
        }
        //UDP can't work with dll, so force cycle SLOT function.
        while (true) {
            nr.storeData();
            while (!nr.datagrams.isEmpty()) {
                QByteArray datagram = nr.datagrams.dequeue();
                vm.parse((void*)datagram.data(), datagram.size());
                std::cout << "Some data was gotten from UDP." << "\r";
            }
            Sleep(5);
            std::cout << "Nothing from UDP." << "\r";
        }
    } else {
        qDebug() << "Unknown command. You can get more imformation from README.md";
    }

//    return a.exec();

    a.exit(0);
}
