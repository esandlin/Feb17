/**
 * Copyright (c) 2016 Tim Lindquist,
 * Software Engineering,
 * Arizona State University at the Polytechnic campus
 * <p/>
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2
 * of the License.
 * <p/>
 * This program is distributed in the hope that it will be useful,
 * but without any warranty or fitness for a particular purpose.
 * <p/>
 * Please review the GNU General Public License at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 * see also: https://www.gnu.org/licenses/gpl-faq.html
 * so you are aware of the terms and your rights with regard to this software.
 * Or, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,USA
 * <p/>
 * Purpose: C++ class to represent a Albums. This class is part of a
 * Albums collection distributed application that uses JsonRPC.
 * Meant to run on OSX, Debian Linux, and Raspberry Pi Debian.
 * <p/>
 * Ser321 Principles of Distributed Software Systems
 * see http://pooh.poly.asu.edu/Ser321
 * @author Tim Lindquist (Tim.Lindquist@asu.edu),ASU-IAFSE,Software Engineering
 * @file    Albums.cpp
 * @date    July, 2016
 * @license See above
 **/

#include "Albums.hpp"
#include <iostream>
#include <stdlib.h>
Albums::Albums(){
   name = "";
   album_id = 0;
   takes = vector<string>();
}

Albums::Albums(string aName, int aAlbumId, vector<string> aTakes) {
   name = aName;
   album_id = aAlbumId;
   takes = aTakes;
}

Albums::Albums(const Json::Value& jsonObj){
   string nameStr = "name";
   string albumIdStr = "album_id";
   string takesStr = "takes";
   Json::Value::Members mbr = jsonObj.getMemberNames();
   for(vector<string>::const_iterator i = mbr.begin(); i!= mbr.end(); i++){
      Json::Value jsonM = jsonObj[*i];
      if(nameStr.compare(*i)==0){
         name = jsonM.asString();
      }else if(albumIdStr.compare(*i)==0){
         album_id = jsonM.asInt();
      }else if(takesStr.compare(*i)==0){
         takes = vector<string>();
         for(int i=0; i<jsonM.size(); i++){
            takes.push_back(jsonM[i].asString());
         }
      }
   }
}

Albums::Albums(string jsonString){
   string nameStr = "name";
   string albumIdStr = "album_id";
   string takesStr = "takes";
   Json::Reader reader;
   Json::Value root;
   bool parseSuccess = reader.parse(jsonString,root,false);
   if(parseSuccess){
      Json::Value::Members mbr = root.getMemberNames();
      for(vector<string>::const_iterator i = mbr.begin(); i!= mbr.end(); i++){
         Json::Value jsonM = root[*i];
         if(nameStr.compare(*i)==0){
            name = jsonM.asString();
         }else if(albumIdStr.compare(*i)==0){
            album_id = jsonM.asInt();
         }else if(takesStr.compare(*i)==0){
            takes = vector<string>();
            for(int i=0; i<jsonM.size(); i++){
               takes.push_back(jsonM[i].asString());
            }
         }
      }
   }else{
      cout << "Albums constructor parse error with input: " << jsonString
           << endl;
   }
}

Albums::~Albums() {
   name = "";
   album_id = -1;
}

string Albums::toJsonString(){
   string ret = "{}";
   Json::Value jsonLib;
   jsonLib["name"] = name;
   jsonLib["album_id"] = album_id;
   Json::Value tmp(Json::arrayValue);
   for(int i=0; i<takes.size(); i++){
      tmp[i]=takes[i];
   }
   jsonLib["takes"] = tmp;
   ret = jsonLib.toStyledString();
   return ret;
}

Json::Value Albums::toJson(){
   Json::Value jsonLib;
   jsonLib["name"] = name;
   jsonLib["album_id"] = album_id;
   Json::Value tmp(Json::arrayValue);
   for(int i=0; i<takes.size(); i++){
      tmp[i]=takes[i];
   }
   jsonLib["takes"] = tmp;
   return jsonLib;
}

void Albums::setValues(string aName, int aAlbumId, vector<string> aTakes) {
   name = aName;
   album_id = aAlbumId;
   takes = aTakes;
}

void Albums::print(){
   cout << "Albums " << name << " with ID "
        << album_id << " takes ";
   for (int i=0; i<takes.size(); i++){
      cout << takes[i] << " ";
   }
   cout << endl;
}
