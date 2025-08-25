#pragma once

enum class EntityState {
	Idle,
	Walking,
	Attacking,
	Dashing,
	Dead,
};

enum class Direction {
	Down, // 0
	DownLeft, // 1
	Left, // 2
	UpLeft, // 3
	Up, // 4
	UpRight, // 5
	Right, // 6
	DownRight, // 7
};

struct StateComponent {
    EntityState currentState;
	Direction currentDirection;
    EntityState previousState;
	Direction previousDirection;
    float timer;     // đếm thời gian đã trôi qua
    float duration;  // -1 = vô hạn, >0 = tồn tại trong khoảng thời gian
};
