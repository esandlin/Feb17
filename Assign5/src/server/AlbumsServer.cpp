/**
 * Copyright 2016 Tim Lindquist,
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * <p/>
 * Purpose: C++ class which serves as server for a collection of Albumss. This
 * class is part of a Albums collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist Tim.Lindquist@asu.edu
 *         Software Engineering, CIDSE, IAFSE, ASU Poly
 * @version July 2016
 */

#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <json/json.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <csignal>

#include "albumserverstub.h"
#include "AlbumsCollection.hpp"

using namespace jsonrpc;
using namespace std;

class AlbumsServer : public albumserverstub {
public:
   AlbumsServer(AbstractServerConnector &connector, int port);
   virtual std::string serviceInfo();
   virtual bool saveToJsonFile();
   virtual bool resetFromJsonFile();
   virtual bool add(const Json::Value& mediaName);
   virtual bool remove(const std::string& mediaName);
   virtual Json::Value get(const std::string& mediaName);
   virtual Json::Value getNames();
   virtual std::string getById(int AlbumsId);
   
private:
   AlbumsCollection * library;
   int portNum;
};

AlbumsServer::AlbumsServer(AbstractServerConnector &connector, int port) :
                             albumserverstub(connector){
   library = new AlbumsCollection();
   library->resetFromJsonFile("media.json");
   portNum = port;
}

string AlbumsServer::serviceInfo(){
   std::string msg =
                "Albums collection management service.";
   stringstream ss;
   ss << portNum;
   cout << "serviceInfo called. Returning: Albums collection management service."
        << endl;
   return  msg.append(ss.str());
}

bool AlbumsServer::saveToJsonFile(){
   cout << "saving collection to Albumss.json" << endl;
   bool ret = library->saveToJsonFile("media.json");
   return ret;
}

bool AlbumsServer::resetFromJsonFile(){
   cout << "restoring collection from Albumss.json" << endl;
   bool ret = library->resetFromJsonFile("media.json");
   return ret;
}

bool AlbumsServer::add(const Json::Value& AlbumsName) {
   cout << "Adding " << AlbumsName << endl;
   bool ret = library->add(AlbumsName);
   return ret;
}

bool AlbumsServer::remove(const string& AlbumsName) {
   cout << "Removing " << AlbumsName << endl;
   bool ret = library->remove(AlbumsName);
   return ret;
}

Json::Value AlbumsServer::get(const string& AlbumsName){
   cout << "Getting " << AlbumsName << endl;
   return library->get(AlbumsName);
}

Json::Value AlbumsServer::getNames(){
   Json::FastWriter fw;
   std::string names = fw.write(library->getNames());
   cout << "Get names returning: " << names  << endl;
   return library->getNames();
}

std::string AlbumsServer::getById(int AlbumsId){
   cout << "getById returning: " << library->getById(AlbumsId) << endl;
   return library->getById(AlbumsId);
}

void exiting(){
   std::cout << "Server has been terminated. Exiting normally" << endl;
   //ss.StopListening();
}

int main(int argc, char * argv[]) {
   // invoke with ./bin/AlbumsRPCServer 1099
   int port = 1099;
   if(argc > 1){
      port = atoi(argv[1]);
   }
   HttpServer httpserver(port);
   AlbumsServer ss(httpserver, port);
   std::atexit(exiting);
   auto ex = [] (int i) {cout << "server terminating with signal " << i << endl;
                         // ss.StopListening();
                         exit(0);
                         //return 0;
                        };
   // ^C
   std::signal(SIGINT, ex);
   // abort()
   std::signal(SIGABRT, ex);
   // sent by kill command
   std::signal(SIGTERM, ex);
   // ^Z
   std::signal(SIGTSTP, ex);
   cout << "Albums collection server listening on port " << port
      //<< " press return/enter to quit." << endl;
        << " use ps to get pid. To quit: kill -9 pid " << endl;
   ss.StartListening();
   while(true){
   }
   //int c = getchar();
   ss.StopListening();
   return 0;
}
