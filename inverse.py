import numpy as np

a1 = 350
a2 = 25
a3 = 222
a4 = 222
a5 = 135

def ikm(x, y, z, T5, T6):
    # Adjust input coordinates
    x += 0.001
    y += 0.001
    T4 = np.arctan2(y,x)
    T4 = T4*180/np.pi - 180
    T5 -=180

    # Convert angles from degrees to radians
    T4 = T4 / 180 * np.pi
    T5 = T5 / 180 * np.pi  # Fixed at 180 degrees as given in the original code
    T6 = T6 / 180 * np.pi
    
    # Rotation matrix R0_5
    R0_5 = np.array([
        [np.cos(T4) * np.cos(T5) * np.cos(T6) - np.sin(T4) * np.sin(T6), -np.cos(T4) * np.cos(T5) * np.sin(T6) - np.sin(T4) * np.cos(T6), np.cos(T4) * np.sin(T5)],
        [np.sin(T4) * np.cos(T5) * np.cos(T6) + np.cos(T4) * np.sin(T6), -np.sin(T4) * np.cos(T5) * np.sin(T6) + np.cos(T4) * np.cos(T6), np.sin(T4) * np.sin(T5)],
        [-np.sin(T5) * np.cos(T6), np.sin(T5) * np.sin(T6), np.cos(T5)]
    ])
    
    # End-effector position
    p0_6 = np.array([[x], [y], [z]])
    
    # Calculate R0_5 * [0, 0, a5]'
    R0_5_0 = a5 * R0_5
    R0_5_1 = np.dot(R0_5_0, np.array([[0], [0], [1]]))
    
    # Calculate the position of p0_4
    p0_4 = p0_6 - R0_5_1
    T1 = np.arctan2(p0_4[1], p0_4[0])

    b = p0_4[2] - a1
    b = float(b)
    c = np.sqrt(p0_4[0]**2 + p0_4[1]**2)-a2
    c=float(c)
    cosT3 = ((b**2)+(c**2)-(a3**2)-(a4**2))/(2*a3*a4)

    T3 = np.arctan2(-np.sqrt(1-(cosT3**2)),cosT3)
    T2 = np.arctan2(b,c) - np.arctan2(a4*np.sin(T3), a3 + a4*np.cos(T3)) #Вычисление углов поворота первых 3 осей

    T1=float(T1)
    T2=float(T2)
    T3=float(T3)

    PT = [[T1,(90/180)*np.pi,a2,a1],[T2,0,a3,0],[T3,0,a4,0]]
    N0_1=[[np.cos(PT[0][0]),0,np.sin(PT[0][0]),(PT[0][2])*np.cos(PT[0][0])],
        [np.sin(PT[0][0]),0,-np.cos(PT[0][0]),(PT[0][2])*np.sin(PT[0][0])],
        [0,1,0,(PT[0][3])],
        [0,0,0,1]]
    N1_2=[[np.cos(PT[1][0]),-np.sin(PT[1][0]),0,PT[1][2]*np.cos(PT[1][0])],
        [np.sin(PT[1][0]),np.cos(PT[1][0]),0,PT[1][2]*np.sin(PT[1][0])],
        [0,0,1,PT[1][3]],
        [0,0,0,1]]
    N2_3=[[np.cos(PT[2][0]),-np.sin(PT[2][0]),0,PT[2][2]*np.cos(PT[2][0])],
        [np.sin(PT[2][0]),np.cos(PT[2][0]),0,PT[2][2]*np.sin(PT[2][0])],
        [0,0,1,PT[2][3]],
        [0,0,0,1]]

    N0_2 = np.dot(N0_1,N1_2)
    N0_3 = np.dot(N0_2,N2_3)

    R0_3 = [[N0_3[0][0],N0_3[0][1],N0_3[0][2]],
            [N0_3[1][0],N0_3[1][1],N0_3[1][2]],
            [N0_3[2][0],N0_3[2][1],N0_3[2][2]]]
    
    invR0_3 = np.linalg.inv(R0_3)
    R3_5 = np.dot(R0_5, invR0_3)

    # T4 = np.arctan2(R3_5[2][1],-R3_5[2][0])  #Вычисление углов поворота 4 и 5 оси
    # T5 = np.arctan2(np.sqrt(1-(R3_5[2][2])**2),R3_5[2][2])  #Вычисление углов поворота 4 и 5 оси
    
    T1=T1*180/np.pi
    T2=T2*180/np.pi
    T3=T3*180/np.pi

    T4=T5*180/np.pi
    T5=T6*180/np.pi

    T3 += T2
    T4 += 180
    
    T1 = round(T1,2)
    T2 = round(T2,2)
    T3 = round(T3,2)
    T4 = round(T4,2)
    T5 = round(T5,2)
    angles = [T1,T2,T3,T4,T5]

    return angles

def dkm(T1,T2,T3,T4,T5):
    T3 = T3-T2
    T4 = T4 - T3 - T2 -90
    
    T1=T1/180*np.pi
    T2=T2/180*np.pi
    T3=T3/180*np.pi
    T4=T4/180*np.pi
    T5=T5/180*np.pi

    PT = [[T1,(90/180)*np.pi,a2,a1],[T2,0,a3,0],[T3,0,a4,0],[T4+(90/180)*np.pi,(90/180)*np.pi,0,0],[T5,0,0,a5]]
    H0_1=[[np.cos(PT[0][0]),0,np.sin(PT[0][0]),PT[0][2]*np.cos(PT[0][0])],
        [np.sin(PT[0][0]),0,-np.cos(PT[0][0]),PT[0][2]*np.sin(PT[0][0])],
        [0,1,0,PT[0][3]],
        [0,0,0,1]]
    H1_2=[[np.cos(PT[1][0]),-np.sin(PT[1][0]),0,PT[1][2]*np.cos(PT[1][0])],
        [np.sin(PT[1][0]),np.cos(PT[1][0]),0,PT[1][2]*np.sin(PT[1][0])],
        [0,0,1,PT[1][3]],
        [0,0,0,1]]
    H2_3=[[np.cos(PT[2][0]),-np.sin(PT[2][0]),0,PT[2][2]*np.cos(PT[2][0])],
        [np.sin(PT[2][0]),np.cos(PT[2][0]),0,PT[2][2]*np.sin(PT[2][0])],
        [0,0,1,0],
        [0,0,0,1]]
    H3_4=[[np.cos(PT[3][0]),0,np.sin(PT[3][0]),0],
        [np.sin(PT[3][0]),0,-np.cos(PT[3][0]),0],
        [0,1,0,0],
        [0,0,0,1]]
    H4_5=[[np.cos(PT[4][0]),-np.sin(PT[4][0]),0,0],
        [np.sin(PT[4][0]),np.cos(PT[4][0]),0,0],
        [0,0,1,PT[4][3]],
        [0,0,0,1]]

    H0_2 = np.dot(H0_1,H1_2)
    H2_4 = np.dot(H2_3,H3_4)
    H0_4 = np.dot(H0_2,H2_4)
    H0_5 = np.dot(H0_4,H4_5)

    x = round(H0_5[0][3],2)
    y = round(H0_5[1][3],2)
    z = round(H0_5[2][3],2)
    cords = [x,y,z]

    # R0_5 = [[H0_5[0][0],H0_5[0][1],H0_5[0][2]],[H0_5[1][0],H0_5[1][1],H0_5[1][2]],[H0_5[2][0],H0_5[2][1],H0_5[2][2]]]
    # fi = np.arctan2(R0_5[0][1],R0_5[0][0])
    # theta = np.arccos(R0_5[2][2])
    # fi = np.degrees(fi)
    # theta = np.degrees(theta)
    # print(fi, theta)
    return cords

def jacobian(x_dot,y_dot,z_dot,T1,T2,T3,T4,T5):

    wx_dot = 0
    wz_dot = 0

    T1 += 0.001
    T3 = T3-T2
    T4 = T4 - T3 - T2 - 90
    T1, T2, T3, T4, T5 = np.deg2rad([T1, T2, T3, T4, T5])

    # Вычисление однородной матрицы преобразования
    PT = [[T1,(90/180)*np.pi,a2,a1],[T2,0,a3,0],[T3,0,a4,0],[T4+(90/180)*np.pi,(90/180)*np.pi,0,0],[T5,0,0,a5]]
    H0_1=np.array([[np.cos(PT[0][0]),0,np.sin(PT[0][0]),PT[0][2]*np.cos(PT[0][0])],
        [np.sin(PT[0][0]),0,-np.cos(PT[0][0]),PT[0][2]*np.sin(PT[0][0])],
        [0,1,0,PT[0][3]],
        [0,0,0,1]])
    H1_2=np.array([[np.cos(PT[1][0]),-np.sin(PT[1][0]),0,PT[1][2]*np.cos(PT[1][0])],
        [np.sin(PT[1][0]),np.cos(PT[1][0]),0,PT[1][2]*np.sin(PT[1][0])],
        [0,0,1,PT[1][3]],
        [0,0,0,1]])
    H2_3=np.array([[np.cos(PT[2][0]),-np.sin(PT[2][0]),0,PT[2][2]*np.cos(PT[2][0])],
        [np.sin(PT[2][0]),np.cos(PT[2][0]),0,PT[2][2]*np.sin(PT[2][0])],
        [0,0,1,0],
        [0,0,0,1]])
    H3_4=np.array([[np.cos(PT[3][0]),0,np.sin(PT[3][0]),0],
        [np.sin(PT[3][0]),0,-np.cos(PT[3][0]),0],
        [0,1,0,0],
        [0,0,0,1]])
    H4_5=np.array([[np.cos(PT[4][0]),-np.sin(PT[4][0]),0,0],
        [np.sin(PT[4][0]),np.cos(PT[4][0]),0,0],
        [0,0,1,PT[4][3]],
        [0,0,0,1]])

     # Произведение матриц
    H0_2 = np.dot(H0_1, H1_2)
    H2_4 = np.dot(H2_3, H3_4)
    H0_4 = np.dot(H0_2, H2_4)
    H0_5 = np.dot(H0_4, H4_5)
    H0_3 = np.dot(H0_2, H2_3)

    multiplier = [0, 0, 1] 

    R1 = multiplier
    R2 = H0_1[:3, 2]
    R3 = H0_2[:3, 2]
    R4 = H0_3[:3, 2]
    R5 = H0_4[:3, 2]

    d0_1 = H0_1[:3, 3]
    d0_2 = H0_2[:3, 3]
    d0_3 = H0_3[:3, 3]
    d0_4 = H0_4[:3, 3]
    d0_5 = H0_5[:3, 3]

    # Вычисление компонент якобиана
    L1 = np.cross(R1, d0_5)
    L2 = np.cross(R2, d0_5 - d0_1)
    L3 = np.cross(R3, d0_5 - d0_2)
    L4 = np.cross(R4, d0_5 - d0_3)
    L5 = np.cross(R5, d0_5 - d0_4)

    L1 = np.array(L1)
    L2 = np.array(L2)
    L3 = np.array(L3)
    L4 = np.array(L4)
    L5 = np.array(L5)

    R1 = np.array(R1)
    R2 = np.array(R2)
    R3 = np.array(R3)
    R4 = np.array(R4)
    R5 = np.array(R5)

    J1 = np.vstack((L1.reshape(-1,1),R1.reshape(-1,1)))
    J2 = np.vstack((L2.reshape(-1,1),R2.reshape(-1,1)))
    J3 = np.vstack((L3.reshape(-1,1),R3.reshape(-1,1)))
    J4 = np.vstack((L4.reshape(-1,1),R4.reshape(-1,1)))
    J5 = np.vstack((L5.reshape(-1,1),R5.reshape(-1,1)))

    J = np.concatenate((J1,J2,J3,J4,J5),axis=1)
    J = np.delete(J,5,axis=0)
    # Вычисление матрицы Якоби
    if np.linalg.det(J) == 0:
        print("Матрица якобиана сингулярна. Решение может быть неустойчивым.")
        # Возвращаем None вместо обратной матрицы и завершаем функцию
        return None
    J_inv = np.linalg.inv(J)

    end_effector_velocities = np.array([x_dot, y_dot, z_dot, wx_dot, wz_dot])
    # Решение угловых скоростей суставов
    joint_velocities = np.dot(J_inv, end_effector_velocities)

    theta1_dot = joint_velocities[0]
    theta2_dot = joint_velocities[1]
    theta3_dot = joint_velocities[2]
    theta4_dot = joint_velocities[3]
    theta5_dot = joint_velocities[4]

    theta1_dot = theta1_dot*180/np.pi
    theta2_dot = theta2_dot*180/np.pi
    theta3_dot = theta3_dot*180/np.pi
    theta4_dot = theta4_dot*180/np.pi
    theta5_dot = theta5_dot*180/np.pi

    theta4_dot = 0
    theta5_dot = 0

    velocities = [theta1_dot, theta2_dot, theta3_dot, theta4_dot, theta5_dot]
    
    return velocities

def path_planning(x1, y1, z1, x2, y2, z2, v, a, t, t_full, L, Llast):
    d = ((x2 - x1) / L, (y2 - y1) / L, (z2 - z1) / L)  # Направление движения
    
    t_accel = v / a  # Время на разгон
    t_decel = v / a  # Время на торможение
    s_accel = 0.5 * a * t_accel**2  # Расстояние разгона
    s_decel = 0.5 * a * t_decel**2  # Расстояние торможения
    s_constant_speed = L - (s_accel + s_decel)  # Расстояние на постоянной скорости
    
    if t <= t_accel:  # Ускорение
        s = 0.5 * a * t**2
    elif t <= (t_full - t_decel):  # Постоянная скорость
        t_mid = t - t_accel
        s = s_accel + v * t_mid
    else:  # Замедление
        t_dec = t - (t_full - t_decel)
        s = L - (0.5 * a * (t_decel - t_dec)**2)
    
    # Вычисление позиции
    x = x1 + s * d[0]
    y = y1 + s * d[1]
    z = z1 + s * d[2]
    
    return [x, y, z]

def velocity_planning(x1,y1,z1,x2,y2,z2,v):

    x_dot = v*(x2-x1)/np.sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)+(z2-z1)*(z2-z1))
    y_dot = v*(y2-y1)/np.sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)+(z2-z1)*(z2-z1))
    z_dot = v*(z2-z1)/np.sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)+(z2-z1)*(z2-z1))
    A = [x_dot,y_dot,z_dot]
    return A