#pragma once
#include "sl.h"
#include <iostream>
#include <stdbool.h>
#include <windows.h>
#include <chrono>
#include <thread>
#include "json.hpp"
#include <cpr.h>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include <algorithm>
#include <locale>
std::string FormatWithCommas(double value)
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << trunc(value);
	return ss.str();
}
void Auth();
void CallAPI();
#pragma warning(disable:4996)
static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
	if (userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if (os.write(static_cast<char*>(buf), len))
			return len;
	}

	return 0;
}
CURLcode curl_read(const std::string& url, std::ostream& os, long timeout = 30)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();

	if (curl)
	{
		if (CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
		{
			code = curl_easy_perform(curl);
		}
		curl_easy_cleanup(curl);
	}
	return code;
}
void ReplaceStringInPlace(std::string& subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}
std::string clientId = "0", clientSecret = "key", accessToken = "key", auth = "shit dont know", userID = "1234567890", mode = "osu";
std::string username = "username", rankedScore = "0", totalScore = "0", scoreRank = "0", playTime = "0", playCount = "0", silverSS = "0", goldSS = "0", silverS = "0", goldS = "0", rankA = "0";

std::string XrankedScore = "0", XtotalScore = "0", XscoreRank = "0", XplayTime = "0", XplayCount = "0", XsilverSS = "0", XgoldSS = "0", XsilverS = "0", XgoldS = "0", XrankA = "0";
int hour = 0, minute = 0, second = 0, Xhour = 0, Xminute = 0, Xsecond = 0;
int apiCallIntervall = 10000;
using namespace std;
using json = nlohmann::json;
json result;
json resultNew;
json PostResult;
json PostResultNew;
bool imageLoaded = false;
bool hasImage = false;
int main(int args, char* argv[])
{
	Auth();
	std::thread thread_obj(CallAPI);
	slWindow(600, 400, "osu! Tracker", false, 144, 0, 0);
	slSetFont(slLoadFont("ttf/arial.ttf"), 3);
	char fps[99];
	char ms[99];
	char time[99];
	float d;
	bool debugOverlay = false;
	bool start = false;
	double temptime = 0;

	wchar_t buffer[256];
	int pos = 0;
	double pastData[100] = { 0 };
	double pastFPS[100] = { 0 };
	int xs = 1;
	int sleep = 200;
	bool keyf11 = false;
	int xf = 0;
	//int pfp = slLoadTexture("user.jpeg");

	while (!slShouldClose())
	{
		/*	if (ret < 0) {
				return EXIT_FAILURE;
			}
			if (ret >= sizeof fps) {
				// Result was truncated - resize the buffer and retry.
			}*/

			/*	if (slGetKey(SL_KEY_ESCAPE) == 1)
				{
					return 0; // closing
				}

				if (slGetKey(SL_KEY_UP) == 1)
				{
					sleep += 1;
				}
				if (slGetKey(SL_KEY_DOWN) == 1)
				{
					if (sleep > 0) {
						sleep -= 1;
					}
				}
				*/

				//slSprite(pfp, 200, 240, 300, 200);
		slSetFontSize(22);
		slSetBackColor(0.062, 0.078, 0.094);
		slSetForeColor(0.016, 0.032, 0.032, 1);
		slRectangleFill(0, 400, 1200, 60);
		slSetForeColor(0.04, 0.05, 0.06, 1);
		slRectangleFill(0, 185, 600, 370);
		slSetTextAlign(SL_ALIGN_CENTER);
		slSetForeColor(1, 1, 1, 1);
		slText(300, slGetWindowHeight() - slGetTextHeight((username).c_str()), (username).c_str());
		slSetTextAlign(SL_ALIGN_LEFT);
		slSetFontSize(18);
		slText(6, 345, "Score Rank:  #");
		slText(6, 320, ("Play Count: "));

		slText(6, 295, ("Play Time:"));

		slText(6, 270, ("Ranked Score: "));
		slText(6, 245, ("Total Score: "));
		slText(6, 220, ("Silver SS: "));
		slText(6, 195, ("Gold SS: "));
		slText(6, 170, ("Silver S: "));
		slText(6, 145, ("Gold S: "));
		slText(6, 120, ("Rank A: "));
		//VALUE  std::to_string(hour) + "h " + std::to_string(minute) + "m " + std::to_string(second) + "s").c_str()
		slSetForeColor(0.6, 0.6, 1, 1);
		slText(6 + slGetTextWidth("Score Rank:  #"), 345, (scoreRank).c_str());
		slText(6 + slGetTextWidth("Play Count:  "), 320, (playCount).c_str());
		slText(6 + slGetTextWidth("Play Time:  "), 295, (std::to_string(hour)).c_str());
		slSetForeColor(1, 1, 1, 1);
		slText(6 + slGetTextWidth(("Play Time: " + std::to_string(hour)).c_str()), 295, "h");
		slSetForeColor(0.6, 0.6, 1, 1);
		slText(6 + slGetTextWidth(("Play Time: " + std::to_string(hour) + "h  ").c_str()), 295, (std::to_string(minute)).c_str());
		slSetForeColor(1, 1, 1, 1);
		slText(6 + slGetTextWidth(("Play Time: " + std::to_string(hour) + "h " + std::to_string(minute)).c_str()), 295, " m");
		slSetForeColor(0.6, 0.6, 1, 1);
		slText(6 + slGetTextWidth(("Play Time:  " + std::to_string(hour) + "h " + std::to_string(minute) + "m  ").c_str()), 295, (std::to_string(second)).c_str());
		slSetForeColor(1, 1, 1, 1);
		slText(6 + slGetTextWidth(("Play Time:  " + std::to_string(hour) + "h " + std::to_string(minute) + "m  " + std::to_string(second)).c_str()), 295, "s");
		slSetForeColor(0.6, 0.6, 1, 1);
		slText(6 + slGetTextWidth("Ranked Score:  "), 270, (rankedScore).c_str());
		slText(6 + slGetTextWidth("Total Score:  "), 245, (totalScore).c_str());
		slSetForeColor(0.752, 0.752, 0.752, 1); //SILVER
		slText(6 + slGetTextWidth("Silver SS:  "), 220, (silverSS).c_str());
		slSetForeColor(1, 0.843, 0, 1); //GOLD
		slText(6 + slGetTextWidth("Gold SS:  "), 195, (goldSS).c_str());
		slSetForeColor(0.752, 0.752, 0.752, 1);
		slText(6 + slGetTextWidth("Silver S:  "), 170, (silverS).c_str());
		slSetForeColor(1, 0.843, 0, 1); //GOLD
		slText(6 + slGetTextWidth("Gold S:  "), 145, (goldS).c_str());
		slSetForeColor(0.6, 0.6, 1, 1);
		slText(6 + slGetTextWidth("Rank A:  "), 120, (rankA).c_str());
		slSetForeColor(1, 1, 1, 0.1);
		slLine(0, 339, 600, 339);
		slLine(0, 339 - (25 * 1), 600, 339 - (25 * 1));
		slLine(0, 339 - (25 * 2), 600, 339 - (25 * 2));
		slLine(0, 339 - (25 * 3), 600, 339 - (25 * 3));
		slLine(0, 339 - (25 * 4), 600, 339 - (25 * 4));
		slLine(0, 339 - (25 * 5), 600, 339 - (25 * 5));
		slLine(0, 339 - (25 * 6), 600, 339 - (25 * 6));
		slLine(0, 339 - (25 * 7), 600, 339 - (25 * 7));
		slLine(0, 339 - (25 * 8), 600, 339 - (25 * 8));
		slLine(0, 339 - (25 * 9), 600, 339 - (25 * 9));
		//CHANGES
		slSetForeColor(1, 1, 1, 1);
		slText(6 + 300, 345, (XscoreRank).c_str());
		slText(6 + 300, 320, (XplayCount).c_str());
		slText(6 + 300, 295, ((std::to_string(Xhour) + "h " + std::to_string(Xminute) + "m " + std::to_string(Xsecond) + "s").c_str()));
		slText(6 + 300, 270, (XrankedScore).c_str());
		slText(6 + 300, 245, (XtotalScore).c_str());
		slText(6 + 300, 220, (XsilverSS).c_str());
		slText(6 + 300, 195, (XgoldSS).c_str());
		slText(6 + 300, 170, (XsilverS).c_str());
		slText(6 + 300, 145, (XgoldS).c_str());
		slText(6 + 300, 120, (XrankA).c_str());

		if (slGetKey(SL_KEY_F11))
		{
			if (!keyf11)
			{
				keyf11 = true;
				debugOverlay = !debugOverlay;
			}
		}
		else {
			keyf11 = false;
		}

		if (debugOverlay) {
			slSetForeColor(0, 0, 0, 1);
			slRectangleFill(0, 185, 600, 370);
			d = 1000 / slGetDeltaTime();
			d = d / 1000;
			snprintf(fps, sizeof fps, "%4.2f", d);
			snprintf(ms, sizeof ms, "%4.2f", slGetDeltaTime() * 1000);
			snprintf(time, sizeof time, "%4.2f", slGetTime());

			if (xf == 100) {
				xf = 0;
			}
			pos = 0;
			xs = 0;
		label:

			if (pastData[xs] * 10000 > pastFPS[xs] / 10) {
				if (pastData[xs] * 1000 <= 16.5) {
					slSetForeColor(1, 1, 1, 1);
				}
				if (pastData[xs] * 1000 > 16.5) {
					slSetForeColor(1, 1, 0, 1);
				}
				if (pastData[xs] * 1000 > 33) {
					slSetForeColor(1, 0, 0, 1);
				}
				slLine(pos, 0, pos, pastData[xs] * 1000);
				slSetForeColor(0, 0, 255, 1);
				slLine(pos, 0, pos, pastFPS[xs] / 10);
			}
			else {
				slSetForeColor(0, 0, 255, 1);
				slLine(pos, 0, pos, pastFPS[xs] / 10);
				if (pastData[xs] * 1000 <= 16.5) {
					slSetForeColor(1, 1, 1, 1);
				}
				if (pastData[xs] * 1000 > 16.5) {
					slSetForeColor(1, 1, 0, 1);
				}
				if (pastData[xs] * 1000 > 33) {
					slSetForeColor(1, 0, 0, 1);
				}
				slLine(pos, 0, pos, pastData[xs] * 1000);
			}

			//	slRectangleFill(pos, 0, 1, );

			pos += 1;
			xs += 1;
			if (xs == 100) {
				goto endloop;
			}
			goto label;
		endloop:

			//	if ((slGetTime() - temptime) > 0.1) {
			pastData[xf] = slGetDeltaTime();
			pastFPS[xf] = atof(fps);
			xf++;

			temptime = slGetTime();

			strcat_s(fps, "F/s");
			strcat_s(ms, "ms");
			slSetForeColor(1, 1, 1, 1.0);
			slText(0 + 2, 400 - 25, "FPS:");
			slText(0 + 2, 400 - 55, "Render Latency:");
			slText(0 + 2, 400 - 80, "Time:");
			slText(0 + 45, 400 - 25, fps);
			slText(0 + 140, 400 - 55, ms);
			slText(0 + 55, 400 - 80, time);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
		slRender();
	}

	return 0;
}

void Auth() {
	//using json = nlohmann::json;
	//GET CLIENT_ID AND CLIENT_SECRET FROM CONFIG FILE
	std::stringstream buffer;
	std::ifstream input("config.txt");
	for (std::string line; getline(input, line); )
	{
		if (line.find("client_id=") != std::string::npos) {
			ReplaceStringInPlace(line, "client_id=", "");
			clientId = line;
		}

		if (line.find("client_secret=") != std::string::npos) {
			ReplaceStringInPlace(line, "client_secret=", "");
			clientSecret = line;
		}

		if (line.find("mode=") != std::string::npos) {
			ReplaceStringInPlace(line, "mode=", "");
			mode = line;
		}

		if (line.find("user_ID=") != std::string::npos) {
			ReplaceStringInPlace(line, "user_ID=", "");
			userID = line;
		}

		if (line.find("APICallIntervall=") != std::string::npos) {
			ReplaceStringInPlace(line, "APICallIntervall=", "");
			apiCallIntervall = atoi(line.c_str());
		}
	}
	std::string body = R"({"client_id":)" + clientId + R"(, "client_secret":")" + clientSecret + R"(", "grant_type":"client_credentials", "scope":"public"})";
	auto r = cpr::Post(cpr::Url{ "https://osu.ppy.sh/oauth/token" },
		cpr::Body{
			body
		},
		cpr::Header{ { "Content-Type", "application/json" } });
	PostResult = json::parse(r.text);
	std::string tokenKey = PostResult.value("access_token", "");
	auth = "Bearer " + tokenKey;

	/*	auto g = cpr::Get(cpr::Url{"https://osu.ppy.sh/api/v2/users/" + userID + "/" + mode + "?key=id"},
			cpr::Header{
				{ "Content-Type", "application/json" },
				{ "Accept", "application/json" },
				{ "Authorization", auth}
			});*/

			//PostResult = json::parse(g.text);

		/*	string url = PostResult.value("avatar_url", "");

			curl_global_init(CURL_GLOBAL_ALL);

			std::ofstream ofs("/image/user.jpeg", std::ostream::binary);

			if (CURLE_OK == curl_read(url, ofs))
			{
			}

			curl_global_cleanup();
		*/

		//GET USER DETAILS
	auto g = cpr::Get(cpr::Url{ "https://osu.ppy.sh/api/v2/users/" + userID + "/" + mode + "?key=id" },
		cpr::Header{
			{ "Content-Type", "application/json" },
			{ "Accept", "application/json" },
			{ "Authorization", auth}
		});
	PostResult = json::parse(g.text);
	username = PostResult.value("username", "");

	buffer << PostResult["statistics"]["play_time"];
	playTime = buffer.str();
	buffer.str("");

	buffer << PostResult["statistics"]["ranked_score"];
	rankedScore = buffer.str();
	rankedScore = FormatWithCommas(std::stod(rankedScore.c_str()));
	std::replace(rankedScore.begin(), rankedScore.end(), ',', '.');
	ReplaceStringInPlace(rankedScore, ".000000", "");
	buffer.str("");

	buffer << PostResult["statistics"]["total_score"];
	totalScore = buffer.str();
	totalScore = FormatWithCommas(std::stod(totalScore.c_str()));
	std::replace(totalScore.begin(), totalScore.end(), ',', '.');
	ReplaceStringInPlace(totalScore, ".000000", "");
	buffer.str("");

	buffer << PostResult["statistics"]["play_count"];
	playCount = buffer.str();
	playCount = FormatWithCommas(atoi(playCount.c_str()));
	std::replace(playCount.begin(), playCount.end(), ',', '.');
	ReplaceStringInPlace(playCount, ".000000", "");
	buffer.str("");

	buffer << PostResult["statistics"]["grade_counts"]["ssh"];
	silverSS = buffer.str();
	silverSS = FormatWithCommas(atoi(silverSS.c_str()));
	std::replace(silverSS.begin(), silverSS.end(), ',', '.');
	ReplaceStringInPlace(silverSS, ".000000", "");
	buffer.str("");

	auto reskpektive = cpr::Get(cpr::Url{ "https://score.respektive.pw/u/" + userID + "?mode=" + mode });
	result = json::parse(reskpektive.text);

	buffer << result[0].value("rank", 0);
	scoreRank = buffer.str();
	scoreRank = FormatWithCommas(atoi(scoreRank.c_str()));
	std::replace(scoreRank.begin(), scoreRank.end(), ',', '.');
	ReplaceStringInPlace(scoreRank, ".000000", "");
	buffer.str("");

	std::cout << playTime << std::endl;
	int timeplay = atoi(playTime.c_str());
	hour = (int)(timeplay / 3600);
	minute = (int)((timeplay - hour * 3600) / 60);
	second = (int)(timeplay - hour * 3600) - (minute * 60);

	buffer << PostResult["statistics"]["grade_counts"]["ss"];
	goldSS = buffer.str();
	goldSS = FormatWithCommas(atoi(goldSS.c_str()));
	std::replace(goldSS.begin(), goldSS.end(), ',', '.');
	ReplaceStringInPlace(goldSS, ".000000", "");
	buffer.str("");

	buffer << PostResult["statistics"]["grade_counts"]["s"];
	goldS = buffer.str();
	goldS = FormatWithCommas(atoi(goldS.c_str()));
	std::replace(goldS.begin(), goldS.end(), ',', '.');
	ReplaceStringInPlace(goldS, ".000000", "");
	buffer.str("");

	buffer << PostResult["statistics"]["grade_counts"]["sh"];
	silverS = buffer.str();
	silverS = FormatWithCommas(atoi(silverS.c_str()));
	std::replace(silverS.begin(), silverS.end(), ',', '.');
	ReplaceStringInPlace(silverS, ".000000", "");
	buffer.str("");

	buffer << PostResult["statistics"]["grade_counts"]["a"];
	rankA = buffer.str();
	rankA = FormatWithCommas(atoi(rankA.c_str()));
	std::replace(rankA.begin(), rankA.end(), ',', '.');
	ReplaceStringInPlace(rankA, ".000000", "");
	buffer.str("");
}

void CallAPI() {
	std::stringstream buffer;
	double a, b, d;
	string temp;
	while (true) {
		//GET USER DETAILS
		auto g = cpr::Get(cpr::Url{ "https://osu.ppy.sh/api/v2/users/" + userID + "/" + mode + "?key=id" },
			cpr::Header{
				{ "Content-Type", "application/json" },
				{ "Accept", "application/json" },
				{ "Authorization", auth}
			});
		PostResultNew = json::parse(g.text);

		//RANKED SCORE
		buffer << PostResultNew["statistics"]["ranked_score"];
		XrankedScore = buffer.str();
		buffer.str("");
		buffer << PostResult["statistics"]["ranked_score"];
	    temp = buffer.str();
		buffer.str("");
		 a = std::stod(XrankedScore.c_str());
		 b = std::stod(temp.c_str());
		rankedScore = XrankedScore;
		 d = a - b;
		XrankedScore = std::to_string(d);
		XrankedScore = FormatWithCommas(std::stod(XrankedScore.c_str()));
		std::replace(XrankedScore.begin(), XrankedScore.end(), ',', '.');
		ReplaceStringInPlace(XrankedScore, ".000000", "");
		rankedScore = FormatWithCommas(std::stod(rankedScore.c_str()));
		std::replace(rankedScore.begin(), rankedScore.end(), ',', '.');
		ReplaceStringInPlace(rankedScore, ".000000", "");
		temp = "";

		//TOTAL SCORE
		buffer << PostResultNew["statistics"]["total_score"];
		XtotalScore = buffer.str();
		buffer.str("");
		buffer << PostResult["statistics"]["total_score"];
		temp = buffer.str();
		buffer.str("");
		 a = std::stod(XtotalScore.c_str());
		 b = std::stod(temp.c_str());
		totalScore = XtotalScore;
		 d = a - b;
		XtotalScore = std::to_string(d);
		XtotalScore = FormatWithCommas(std::stod(XtotalScore.c_str()));
		std::replace(XtotalScore.begin(), XtotalScore.end(), ',', '.');
		ReplaceStringInPlace(XtotalScore, ".000000", "");
		totalScore = FormatWithCommas(std::stod(totalScore.c_str()));
		std::replace(totalScore.begin(), totalScore.end(), ',', '.');
		ReplaceStringInPlace(totalScore, ".000000", "");
		temp = "";

		//PLAY COUNT
		buffer << PostResultNew["statistics"]["play_count"];
		XplayCount = buffer.str();
		buffer.str("");
		buffer << PostResult["statistics"]["play_count"];
		temp = buffer.str();
		buffer.str("");
		a = std::stod(XplayCount.c_str());
		b = std::stod(temp.c_str());
		playCount = XplayCount;
		d = a - b;
		XplayCount = std::to_string(d);
		XplayCount = FormatWithCommas(std::stod(XplayCount.c_str()));
		std::replace(XplayCount.begin(), XplayCount.end(), ',', '.');
		ReplaceStringInPlace(XplayCount, ".000000", "");
		playCount = FormatWithCommas(std::stod(playCount.c_str()));
		std::replace(playCount.begin(), playCount.end(), ',', '.');
		ReplaceStringInPlace(playCount, ".000000", "");
		temp = "";



	//SILVER SS
		buffer << PostResultNew["statistics"]["grade_counts"]["ssh"];
		XsilverSS = buffer.str();
		buffer.str("");
		buffer << PostResult["statistics"]["grade_counts"]["ssh"];
		temp = buffer.str();
		buffer.str("");
		a = std::stod(XsilverSS.c_str());
		b = std::stod(temp.c_str());
		silverSS = XsilverSS;
		d = a - b;
		if (d < 0) {
			XsilverSS = std::to_string(d);
			XsilverSS = FormatWithCommas(std::stod(XsilverSS.c_str()));
			std::replace(XsilverSS.begin(), XsilverSS.end(), ',', '.');
			ReplaceStringInPlace(XsilverSS, ".000000", "");
			
		}
		else {
			XsilverSS = std::to_string(d);
			XsilverSS = FormatWithCommas(std::stod(XsilverSS.c_str()));
			std::replace(XsilverSS.begin(), XsilverSS.end(), ',', '.');
			ReplaceStringInPlace(XsilverSS, ".000000", "");
		//	XsilverSS = "+ " + XsilverSS;
		}
		silverSS = FormatWithCommas(std::stod(silverSS.c_str()));
		std::replace(silverSS.begin(), silverSS.end(), ',', '.');
		ReplaceStringInPlace(silverSS, ".000000", "");
		temp = "";


		// SCORE RANK
		auto reskpektive = cpr::Get(cpr::Url{ "https://score.respektive.pw/u/" + userID + "?mode=" + mode });
		resultNew = json::parse(reskpektive.text);
		buffer << resultNew[0].value("rank", 0);
		XscoreRank = buffer.str();
		buffer.str("");
		buffer << result[0].value("rank", 0);
		temp = buffer.str();
		buffer.str("");
		a = std::stod(XscoreRank.c_str());
		b = std::stod(temp.c_str());
		scoreRank = XscoreRank;
		d = a - b;
		
		if (d < 0) {
			XscoreRank = std::to_string(d);
			XscoreRank = FormatWithCommas(std::stod(XscoreRank.c_str()));
			//std::replace(XscoreRank.begin(), XscoreRank.end(), ',', '.');
			ReplaceStringInPlace(XscoreRank, ".000000", "");

		}
		else {
			XscoreRank = std::to_string(d);
			XscoreRank = FormatWithCommas(std::stod(XscoreRank.c_str()));
		//	std::replace(XsilverSS.begin(), XscoreRank.end(), ',', '.');
			ReplaceStringInPlace(XscoreRank, ".000000", "");
			
		}
		
		scoreRank = FormatWithCommas(std::stod(scoreRank.c_str()));
		std::replace(scoreRank.begin(), scoreRank.end(), ',', '.');
		ReplaceStringInPlace(scoreRank, ".000000", "");
		temp = "";


		
		//PLAY TIME
		int timeplay;
		buffer << PostResultNew["statistics"]["play_time"];
		XplayTime = buffer.str();
		buffer.str("");
		buffer << PostResult["statistics"]["play_time"];
		temp = buffer.str();
		buffer.str("");
		a = std::stod(XplayTime.c_str());
		b = std::stod(temp.c_str());
		playTime = XplayTime;
		d = a - b;

	    timeplay = atoi(XplayTime.c_str());
		hour = (int)(timeplay / 3600);
		minute = (int)((timeplay - hour * 3600) / 60);
		second = (int)(timeplay - hour * 3600) - (minute * 60);

		timeplay = (atoi(XplayTime.c_str()) - atoi(temp.c_str()));
		Xhour = (int)(timeplay / 3600);
		Xminute = (int)((timeplay - Xhour * 3600) / 60);
		Xsecond = (int)(timeplay - Xhour * 3600) - (Xminute * 60);
		temp = "";














		// GOLD SS
		buffer << PostResultNew["statistics"]["grade_counts"]["ss"];
		XgoldSS = buffer.str();
		buffer.str("");
		buffer << PostResult["statistics"]["grade_counts"]["ss"];
		temp = buffer.str();
		buffer.str("");
		a = std::stod(XgoldSS.c_str());
		b = std::stod(temp.c_str());
		goldSS = XgoldSS;
		d = a - b;
		if (d < 0) {
			XgoldSS = std::to_string(d);
			XgoldSS = FormatWithCommas(std::stod(XgoldSS.c_str()));
			std::replace(XgoldSS.begin(), XgoldSS.end(), ',', '.');
			ReplaceStringInPlace(XgoldSS, ".000000", "");
			
		}
		else {
			XgoldSS = std::to_string(d);
			XgoldSS = FormatWithCommas(std::stod(XgoldSS.c_str()));
			std::replace(XgoldSS.begin(), XgoldSS.end(), ',', '.');
			ReplaceStringInPlace(XgoldSS, ".000000", "");
			XgoldSS = XgoldSS;
		}
		goldSS = FormatWithCommas(std::stod(goldSS.c_str()));
		std::replace(goldSS.begin(), goldSS.end(), ',', '.');
		ReplaceStringInPlace(goldSS, ".000000", "");
		temp = "";




		// GOLD S
		buffer << PostResultNew["statistics"]["grade_counts"]["s"];
		XgoldS = buffer.str();
		buffer.str("");
		buffer << PostResult["statistics"]["grade_counts"]["s"];
		temp = buffer.str();
		buffer.str("");
		a = std::stod(XgoldS.c_str());
		b = std::stod(temp.c_str());
		goldS = XgoldS;
		d = a - b;
		if (d < 0) {
			XgoldS = std::to_string(d);
			XgoldS = FormatWithCommas(std::stod(XgoldS.c_str()));
			std::replace(XgoldS.begin(), XgoldS.end(), ',', '.');
			ReplaceStringInPlace(XgoldS, ".000000", "");

		}
		else {
			XgoldS = std::to_string(d);
			XgoldS = FormatWithCommas(std::stod(XgoldS.c_str()));
			std::replace(XgoldS.begin(), XgoldS.end(), ',', '.');
			ReplaceStringInPlace(XgoldS, ".000000", "");
			XgoldS = XgoldS;
		}
		goldS = FormatWithCommas(std::stod(goldS.c_str()));
		std::replace(goldS.begin(), goldS.end(), ',', '.');
		ReplaceStringInPlace(goldS, ".000000", "");
		temp = "";

		// SILVER S
		buffer << PostResultNew["statistics"]["grade_counts"]["sh"];
		XsilverS = buffer.str();
		buffer.str("");
		buffer << PostResult["statistics"]["grade_counts"]["sh"];
		temp = buffer.str();
		buffer.str("");
		a = std::stod(XsilverS.c_str());
		b = std::stod(temp.c_str());
		silverS = XsilverS;
		d = a - b;
		if (d < 0) {
			XsilverS = std::to_string(d);
			XsilverS = FormatWithCommas(std::stod(XsilverS.c_str()));
			std::replace(XsilverS.begin(), XsilverS.end(), ',', '.');
			ReplaceStringInPlace(XsilverS, ".000000", "");

		}
		else {
			XsilverS = std::to_string(d);
			XsilverS = FormatWithCommas(std::stod(XsilverS.c_str()));
			std::replace(XsilverS.begin(), XsilverS.end(), ',', '.');
			ReplaceStringInPlace(XsilverS, ".000000", "");
			XsilverS =  XsilverS;
		}
		silverS = FormatWithCommas(std::stod(silverS.c_str()));
		std::replace(silverS.begin(), silverS.end(), ',', '.');
		ReplaceStringInPlace(silverS, ".000000", "");
		temp = "";



		// RANK A
		buffer << PostResultNew["statistics"]["grade_counts"]["a"];
		XrankA = buffer.str();
		buffer.str("");
		buffer << PostResult["statistics"]["grade_counts"]["a"];
		temp = buffer.str();
		buffer.str("");
		a = std::stod(rankA.c_str());
		b = std::stod(temp.c_str());
		rankA = XrankA;
		d = a - b;
		if (d < 0) {
			XrankA = std::to_string(d);
			XrankA = FormatWithCommas(std::stod(XrankA.c_str()));
			std::replace(XrankA.begin(), XrankA.end(), ',', '.');
			ReplaceStringInPlace(XrankA, ".000000", "");

		}
		else {
			XrankA = std::to_string(d);
			XrankA = FormatWithCommas(std::stod(XrankA.c_str()));
			std::replace(XrankA.begin(), XrankA.end(), ',', '.');
			ReplaceStringInPlace(XrankA, ".000000", "");
			XrankA =  XrankA;
		}
		rankA = FormatWithCommas(std::stod(rankA.c_str()));
		std::replace(rankA.begin(), rankA.end(), ',', '.');
		ReplaceStringInPlace(rankA, ".000000", "");
		temp = "";
		
		// DEBUG PURPOSE
		std::cout << "Username:" << username << std::endl;
		std::cout << "Play Time: " << hour << minute << second << std::endl;
		std::cout << "PlayCount: " << playCount << std::endl;
		std::cout << "RankedScore: " << rankedScore << std::endl;
		std::cout << "TotalScore: " << totalScore << std::endl;
		std::cout << "Score Rank: " << scoreRank << std::endl;
		std::cout << "Silver SS: " << silverSS << std::endl;
		std::cout << "Silver S: " << silverS << std::endl;
		std::cout << "Gold SS: " << goldSS << std::endl;
		std::cout << "Gold S: " << goldS << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(apiCallIntervall));
	}
}

