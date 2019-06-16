#pragma once

struct Vec3 {
	double X;
	double Y;
	double Z;
	inline Vec3 operator +=(Vec3 A) {
		X = X + A.X;
		Y = Y + A.Y;
		Z = Z + A.Z;
		return *this;
	}
	Vec3(double X, double Y, double Z) {
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}
	Vec3() {
		X = 0;
		Y = 0;
		Z = 0;
	}
};

inline Vec3 operator -(Vec3 A, Vec3 B) {
	Vec3 ret(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
	return ret;
}

inline Vec3 operator +(Vec3 A, Vec3 B) {
	Vec3 ret;
	ret.X = B.X + A.X;
	ret.Y = B.Y + A.Y;
	ret.Z = B.Z + A.Z;
	return ret;
}

inline Vec3 operator *(Vec3 A, Vec3 B) {
	Vec3 ret;
	ret.X = B.X * A.X;
	ret.Y = B.Y * A.Y;
	ret.Z = B.Z * A.Z;
	return ret;
}

inline Vec3 operator /(Vec3 A, double B) {
	Vec3 ret;
	ret.X = A.X / B;
	ret.Y = A.Y / B;
	ret.Z = A.Z / B;
	return ret;
}

inline Vec3 operator *(Vec3 A, double B) {
	Vec3 ret;
	ret.X = A.X * B;
	ret.Y = A.Y * B;
	ret.Z = A.Z * B;
	return ret;
}
