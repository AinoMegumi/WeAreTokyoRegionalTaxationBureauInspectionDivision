#include"Ini.h"
#include<Shlwapi.h>
#include<string>
#include<fstream>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Kernel32.lib")
constexpr size_t GetPrivateProfileStringBuf = 1024;
std::string getLastErrorText();
static inline void clearErr();

std::string change_full_path(std::string filepath_or_name) {
	if (filepath_or_name == "None") return filepath_or_name;
	else if (':' != filepath_or_name.at(1)) {//2文字目が':'ならフルパス
		char dir[MAX_PATH];
		if (!GetCurrentDirectory(MAX_PATH, dir)) throw std::runtime_error("get current dirctory error.");
		//".\\[filepath_or_name]だとファイルが無いときにcreate_UTF16_fileがCurrent Directoryに生成しない"
		return dir + ("\\" + filepath_or_name);
	}
	else return filepath_or_name;
}


Ini::Ini(std::string const& filepath_or_name) {
	this->filefullpath = change_full_path(filepath_or_name);
	this->was_no_exist = (!PathFileExists(filefullpath.c_str()));
	if (was_no_exist) {
		create_UTF16_file();
	}
}

bool Ini::is_now_created() const {
	return this->was_no_exist;
}

void Ini::create_UTF16_file() {
	if (this->was_no_exist || !PathFileExists(this->filefullpath.c_str())) {
		constexpr int BOM = 0xFEFF;
		std::wofstream wfout;
		wfout.open(this->filefullpath, std::ios_base::binary);
		wfout << BOM;
	}
}

void Ini::edit(std::string const& section, std::string const& key, std::string const& str) {
	WritePrivateProfileString(section.c_str(), key.c_str(), str.c_str(), this->filefullpath.c_str());
}

int Ini::getnum(std::string const& section, std::string const& key, const int default_num) {
	return GetPrivateProfileInt(section.c_str(), key.c_str(), default_num, this->filefullpath.c_str());
}

std::string Ini::getString(std::string const& section, std::string const& key, std::string const& default_str) {
	char buf[GetPrivateProfileStringBuf];
	GetPrivateProfileString(section.c_str(), key.c_str(), default_str.c_str(), buf, GetPrivateProfileStringBuf, this->filefullpath.c_str());
	return std::string(buf);
}

static inline void clearErr() {
	SetLastError(NO_ERROR);
}

std::string getLastErrorText() {
	char* buf = nullptr;
	const auto lasterr = GetLastError();
	const auto len = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		nullptr,
		lasterr,
		LANG_NEUTRAL,
		(LPSTR)&buf,
		0,
		nullptr
	);
	auto i = len - 3;
	for (; '\r' != buf[i] && '\n' != buf[i] && '\0' != buf[i]; i++);//改行文字削除
	buf[i] = '\0';
	std::string ret = buf + ("(" + std::to_string(lasterr)) + ")";//エラーメッセージ作成
	LocalFree(buf);//FormatMessageAでFORMAT_MESSAGE_ALLOCATE_BUFFERを指定したので必ず開放
	return ret;
}
