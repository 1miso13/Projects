DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS reviewer;
DROP TABLE IF EXISTS reviews;
DROP TABLE IF EXISTS reactions;

CREATE TABLE users(
ID_U BIGINT NOT NULL AUTO_INCREMENT,
login_U VARCHAR(255) DEFAULT NULL,
password_U VARCHAR(255) DEFAULT NULL,
email_U VARCHAR(30) NOT NULL,
type_U BIGINT NOT NULL,

FILENAME VARCHAR(255) NOT NULL DEFAULT 'File was not uploaded yet',

PRIMARY KEY(ID_U)
);

CREATE TABLE reviewer(
ID_AUTOR BIGINT NOT NULL,		
ID_RECENZENT BIGINT NOT NULL,	
ID_R BIGINT,
PRIMARY KEY(ID_AUTOR,ID_RECENZENT)
);

CREATE TABLE reviews(
ID_R BIGINT AUTO_INCREMENT,
PRIMARY KEY(ID_R),
ID_RT BIGINT,
answer1 BIGINT,
answer2 VARCHAR(2048),
answer3 BIGINT,
answer4 VARCHAR(2048),
answer5 BIGINT,
answer6 VARCHAR(2048),
answer7 VARCHAR(2048),
answer8 VARCHAR(2048)
);

CREATE TABlE reactions(
ID_RT BIGINT AUTO_INCREMENT,
Reaction VARCHAR(2048) DEFAULT NULL,
PRIMARY KEY(ID_RT)
);

ALTER TABLE reviewer ADD FOREIGN KEY(ID_R) REFERENCES reviews(ID_R);
ALTER TABLE reviews ADD FOREIGN KEY(ID_RT) REFERENCES reactions(ID_RT);

INSERT INTO users(login_U, password_U, email_U, type_U) VALUES ('admin', '12345678', 'admin@myemail.com', '1');
INSERT INTO users(login_U, password_U, email_U, type_U) VALUES ('xklobu01', '78AebcdQ', 'xklobu01@stud.fit.vutbr.cz', '2');
INSERT INTO users(login_U, password_U, email_U, type_U) VALUES ('xondre08', 'Ag12eI0g', 'xondre08@stud.fit.vutbr.cz', '2');
INSERT INTO users(login_U, password_U, email_U, type_U) VALUES ('xchudy03', 'Uue25a78', 'xchudy03@stud.fit.vutbr.cz', '2');
INSERT INTO users(login_U, password_U, email_U, type_U) VALUES ('xhinkt00', '2c55Gucg', 'xhinkt00@stud.fit.vutbr.cz', '2');
INSERT INTO users(login_U, password_U, email_U, type_U) VALUES ('xkissm01', '2c55Gucg', 'xkissm01@stud.fit.vutbr.cz', '2');
INSERT INTO users(login_U, password_U, email_U, type_U) VALUES ('xmichl02', '2c55Gucg', 'xmichl02@stud.fit.vutbr.cz', '2');

INSERT INTO reviewer(ID_AUTOR,ID_RECENZENT) VALUES ('7','2');
INSERT INTO reviewer(ID_AUTOR,ID_RECENZENT) VALUES ('3','4');
INSERT INTO reviewer(ID_AUTOR,ID_RECENZENT) VALUES ('4','5');
INSERT INTO reviewer(ID_AUTOR,ID_RECENZENT) VALUES ('5','6');
INSERT INTO reviewer(ID_AUTOR,ID_RECENZENT) VALUES ('6','7');
