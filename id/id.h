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
	int getID() const;
};

#endif
