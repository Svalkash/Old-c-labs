#pragma once

namespace P5v2
{
	int getTimeMillis(std::chrono::steady_clock::time_point base_time);
	int to_millis(std::chrono::steady_clock::time_point cur_time, std::chrono::steady_clock::time_point base_time);

	class Arc
	{
	private:
		int id;
		int wakeTime;
	public:
		Arc(const int identif = 0, int wt = 0) : id(identif), wakeTime(wt) {}
		void setWakeTime(int wt)
		{
			if (wt < 0)
				throw std::exception("negative_wake_time");
			wakeTime = wt;
		}
		int getWakeTime() const { return wakeTime; }
		int getID() const { return id; }
		bool operator <(const Arc &b) const { return wakeTime < b.wakeTime; }
		bool operator >(const Arc &b) const { return wakeTime > b.wakeTime; }
		bool operator ==(const Arc &b) const { return wakeTime == b.wakeTime; }
		bool operator !=(const Arc &b) const { return wakeTime != b.wakeTime; }
	};
}