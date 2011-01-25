/*
 * Author: youngtrips
 * Created Time:  Tue 25 Jan 2011 04:14:44 PM CST
 * File Name: user.h
 * Description: 
 */

#ifndef __USER_H
#define __USER_H

#include <string.h>

#include "gtk_common.h"
#include "dlist.h"

class User : DList {
	public:
		User(const char *nickname, const char *grpname, const char *avatar,
				const char *mac, const char *ipv4, 
				const char *encoding, const char *moto);

		~User() {}

		bool UpdateNickName(const char *nickname);
		bool UpdateAvatar(const char *avatar);
		bool UpdateMoto(const char *moto);
		bool UpdateGroup(const char *grpname);
	
		const char *GetNickName() { return m_nickname; }
		const char *GetGroupName() { return m_groupname; }
		const char *GetIP() { return m_ipv4; }
		const char *GetMac() { return m_mac; }
		const char *GetEncoding() { return m_encoding; }
		const char *GetMoto() { return m_moto; }
		const char *GetAvatar() { return m_avatar_file; }

		void SetParentTreeIter(GtkTreeIter *iter) { m_parent_iter = iter; }
		void SetMyTreeIter(GtkTreeIter *iter) { m_my_iter = iter; }
		void SetDialog(GtkWidget *dialog) { m_dialog = dialog; }

	private:
		char m_nickname[64];
		char m_avatar_file[128];
		char m_mac[20];
		char m_ipv4[20];
		char m_encoding[16];
		char m_moto[128];
		char m_groupname[32];

		GtkWidget *m_dialog;
		GtkTreeIter *m_parent_iter;
		GtkTreeIter *m_my_iter;
};

#endif
