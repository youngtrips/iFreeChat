/*
 * Author: youngtrips
 * Created Time:  Tue 25 Jan 2011 04:15:14 PM CST
 * File Name: user.cpp
 * Description: 
 */

#include "user.h"

User::User(const char *nickname, const char *grpname, const char *avatar,
		const char *mac, const char *ipv4, 
		const char *encoding, const char *moto) {
	strcpy(m_nickname, nickname);
	strcpy(m_avatar_file, avatar);
	strcpy(m_mac, mac);
	strcpy(m_ipv4, ipv4);
	strcpy(m_encoding, encoding);
	strcpy(m_moto, moto);
	strcpy(m_groupname, grpname);
}

bool User::UpdateNickName(const char *nickname) {
	strcpy(m_nickname, nickname);

	///gtk_tree_store_set_value
	gtk_tree_store_set_value(m_tree_store, m_my_iter, 1, nickname);
	return true;
}


bool User::UpdateAvatar(const char *avatar) {
	strcpy(m_avatar_file, avatar);

	return true;
}

bool User::UpdateMoto(const char *moto) {
	strcpy(m_moto, moto);
	return true;
}

bool User::UpdateGroup(const char *grpname) {
	strcpy(m_groupname, grpname);
	return true;
}
