#ifndef ID_H
#define ID_H

/** Represents a unique ID. */
class ID
{
private:
	static int count;
	int id;

public:
	/** Sets the id of the instance and increments the static count (pattern to preserve uniqueness). */
	ID();

	/** Getters. */
	int getID() const;

	/** This is very dangerous, but hacking my way to make things work. */
	void setID(int id);

	/** This is very dangerous, but hacking my way to make things work. */
	static void setCount(int count);
};

#endif
