/*
 * Author: youngtrips
 * Created Time:  Tue 25 Jan 2011 03:44:01 PM CST
 * File Name: GroupInfo.h
 * Description: 
 */

#ifndef __GROUPINFO_H
#define __GROUPINFO_H

#include "dlist.h"

class Group : DList {
	public:
		Group(const char *grpname);
		~Group();

		User *FindUser(const char *ipv4);
		bool AddUser(const User &user);
		bool DelUser(const char *ipv4);
		GtkTreeIter *GetTreeIter();
		GtkWidget *GetDialog();

	private:
		char m_group_name[64];
		User *m_user_list;

		GtkTreeIter *m_treeiter;
		GtkWidget *m_dialog;
};

class GroupInfo {
	public:
		GroupInfo();
		~GroupInfo();
		Group *FindGroup(const char *grpname);
		bool AddGroup(const char *grpname);
		bool DelGroup(const char *grpname);
	private:
		Group *m_group_list;
		GtkTreeStore *m_treestore;
};

#endif
